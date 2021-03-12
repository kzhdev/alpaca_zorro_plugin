#pragma once

#include <sstream>
#include <string>
#include <cassert>
#include <type_traits>
#include <thread>
#include "alpaca/json.h"
#include "logger.h"
#include "throttler.h"

namespace alpaca {

    extern int(__cdecl* BrokerError)(const char* txt);
    extern int(__cdecl* BrokerProgress)(const int percent);
    extern int(__cdecl* http_send)(char* url, char* data, char* header);
    extern long(__cdecl* http_status)(int id);
    extern long(__cdecl* http_result)(int id, char* content, long size);
    extern void(__cdecl* http_free)(int id);

    template<typename>
    struct is_vector : std::false_type {};

    template<typename T, typename A>
    struct is_vector<std::vector<T, A>> : std::true_type {};

    /**
     * @brief The status of various Alpaca actions.
     */
    enum ActionStatus : uint8_t {
        Open,
        Closed,
        All,
    };

    constexpr const char* to_string(ActionStatus status) {
        constexpr const char* sActionStatus[] = { "open", "closed", "all" };
        assert(status >= ActionStatus::Open && status <= ActionStatus::All);
        return sActionStatus[status];
    }

    template<typename T>
    class Response {
    public:
        explicit Response(int c = 0) noexcept : code_(c), message_("OK") {}
        Response(int c, std::string m) noexcept : code_(c), message_(std::move(m)) {}

        void onError(int c, std::string m) {
            code_ = c;
            message_ = std::move(m);
        }

        int getCode() const noexcept {
            return code_;
        }

        std::string what() const noexcept {
            return message_;
        }

        T& content() noexcept {
            return content_;
        }

        explicit operator bool() const noexcept {
            return code_ == 0;
        }

    private:
        template<typename T, typename CallerT>
        friend Response<T> request(const std::string&, const char*, const char*, Logger* Logger);

        template<typename CallerT>
        void parseContent(const std::string& content) {
            rapidjson::Document d;
            if (d.Parse(content.c_str()).HasParseError()) {
                message_ = "Received parse error when deserializing asset JSON. err=" + std::to_string(d.GetParseError()) + "\n" + content;
                code_ = 1;
                return;
            }

            if (!d.IsObject()) {
                if (d.IsArray()) {
                    if (!is_vector<T>::value) {
                        message_ = "JSON is an arry type, but the response content is an object.";
                        code_ = 1;
                        return;
                    }
                }
                else {
                    message_ = "Deserialized valid JSON but it wasn't an object";
                    code_ = 1;
                    return;
                }
            }
            else if (!std::is_same<CallerT, class Polygon>::value) {
                if (d.HasMember("code") && d.HasMember("message")) {
                    message_ = d["message"].GetString();
                    code_ = d["code"].GetInt();
                    return;
                }
                else if (!d.HasMember("code") && d.HasMember("message") /*&& (strcmp(d["message"].GetString(), "too many requests.") == 0)*/) {
                    message_ = d["message"].GetString();
                    code_ = 1;
                    return;
                }
            }
            else {
                // Check polygon return
                if (d.HasMember("error") && d.HasMember("errorcode")) {
                    message_ = d["error"].GetString();
                    code_ = atoi(d["errorcode"].GetString());
                    return;
                }
            }

            try {
                Parser<rapidjson::Document> parser(d);
                auto result = parse<T, CallerT>(parser, content_);
                code_ = result.first;
                message_ = result.second;
            }
            catch (std::exception& e) {
                code_ = 1;
                message_ = e.what();
            }
        }
        
        template<typename U, typename CallerT>
        std::pair<int, std::string> parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<!is_vector<U>::value>::type* = 0) {
            return content.fromJSON<CallerT>(parser);
        }

        template<typename U, typename CallerT>
        std::pair<int, std::string> parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<is_vector<U>::value>::type* = 0) {
            auto parseArray = [&](auto& arrayObj) -> std::pair<int, std::string> {
                for (auto& item : arrayObj.GetArray()) {
                    if (!item.IsObject()) {
                        assert(false);
                        continue;
                    }
                    auto objJson = item.GetObject();
                    Parser<decltype(item.GetObject())> itemParser(objJson);
                    U::value_type obj;
                    obj.fromJSON<CallerT>(itemParser);
                    content.emplace_back(std::move(obj));
                }
                return std::make_pair(0, "OK");
            };

            if (parser.json.IsArray()) {
                return parseArray(parser.json);
            }
            else if (parser.json.IsObject()) {
                auto& item = parser.json.MemberBegin()->value;
                if (item.IsArray()) {
                    return parseArray(item);
                }
                else {
                    for (auto iter = parser.json.MemberBegin(); iter != parser.json.MemberEnd(); ++iter) {
                        auto& item = iter->value;
                        if (item.IsArray()) {
                            return parseArray(item);
                        }
                    }
                }
            }
            return std::make_pair(0, "OK");
        }

    private:
        int code_;
        std::string message_;
        T content_;
    };


    /**
    * Helper function - Send requst
    * 
    * unfortunately need to make a copy of headers for every request. Otherwise only the first request has headers.
    * 
    */
    template<typename T, typename CallerT>
    inline Response<T> request(const std::string& url, const char* headers = nullptr, const char* data = nullptr, Logger* Logger = nullptr) {
        static Throttler throttler(3);

        if (url.empty()) {
            return  Response<T>(1, "Invalid url - empty");
        }


        if (Logger) {
            Logger->logDebug("--> %s\n", url.c_str());
        }

        while (!throttler.canSent()) {
            // reached throttle limit
            if (!BrokerProgress(1)) {
                return Response<T>(1, "Brokerprogress returned zero. Aborting...");
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(250ms);
        }

        int id = http_send((char*)url.c_str(), (char*)data, (char*)headers);

        if (!id) {
            return Response<T>(1, "Cannot connect to server");
        }

        long n = 0;
        std::stringstream ss;
        while (!(n = http_status(id))) {
            if (!BrokerProgress(1)) {
                http_free(id);
                return Response<T>(1, "Brokerprogress returned zero. Aborting...");
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(250ms);
            // print dots, abort if returns zero.
        }

        if (n > 0) {
            char* buffer = (char*)malloc(n + 1);
            auto received = http_result(id, buffer, n);
            ss << buffer;
            free(buffer); //free up memory allocation
            http_free(id); //always clean up the id!
        }
        else {
            http_free(id); //always clean up the id!
            switch (n) {
            case -2:
                return Response<T>(n, "Id is invalid");
            case -3:
                return Response<T>(n, "Website did not response");
            case -4:
                return Response<T>(n, "Host could not be resolved");
            default:
                return Response<T>(n, "Transfer Failed");
            }
        }

        if (Logger) {
            Logger->logTrace2("<-- %s\n", ss.str().c_str());
        }

        Response<T> response;
        response.parseContent<CallerT>(ss.str());
        return response;
    }

} // namespace alpaca
