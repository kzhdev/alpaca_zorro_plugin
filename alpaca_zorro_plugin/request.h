#pragma once

#include <sstream>
#include <string>
#include <cassert>
#include <type_traits>
#include "alpaca/json.h"
#include "logger.h"

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

    public:
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
        friend Response<T> request(const std::string&, std::string, const char*, Logger* Logger);

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
                else if (!d.HasMember("code") && d.HasMember("message") && (strcmp(d["message"].GetString(), "too many requests.") == 0)) {
                    message_ = d["message"].GetString();
                    code_ = 1;
                    return;
                }
            }
            // TODO: Polygon error

            try {
                Parser<rapidjson::Document> parser(d);
                parse<T, CallerT>(parser, content_);
            }
            catch (std::exception& e) {
                code_ = 1;
                message_ = e.what();
            }
        }
        
        template<typename U, typename CallerT>
        void parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<!is_vector<U>::value>::type* = 0) {
            content.fromJSON<CallerT>(parser);
        }

        template<typename U, typename CallerT>
        void parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<is_vector<U>::value>::type* = 0) {
            auto parseArray = [&](auto& arrayObj) {
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
            };

            if (parser.json.IsArray()) {
                parseArray(parser.json);
            }
            else if (parser.json.IsObject()) {
                auto& item = parser.json.MemberBegin()->value;
                if (item.IsArray()) {
                    parseArray(item);
                }
                else {
                    for (auto iter = parser.json.MemberBegin(); iter != parser.json.MemberEnd(); ++iter) {
                        auto& item = iter->value;
                        if (item.IsArray()) {
                            parseArray(item);
                            break;
                        }
                    }
                }
            }
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
    inline Response<T> request(const std::string& url, std::string headers = "", const char* data = nullptr, Logger* Logger = nullptr) {
        int id = http_send((char*)url.c_str(), (char*)data, (char*)(headers.empty() ? nullptr : headers.c_str()));

        if (!id) {
            return Response<T>(1, "Cannot connect to server");
        }

        long n = 0;
        std::stringstream ss;
        while (!(n = http_status(id))) {
            Sleep(100); // wait for the server to reply
            if (!BrokerProgress(1)) {
                http_free(id);
                return Response<T>(1, "Brokerprogress returned zero. Aborting...");
            }
            // print dots, abort if returns zero.
        }

        if (n > 0) {
            char* buffer = (char*)malloc(n + 1);
            auto received = http_result(id, buffer, n);
            ss << buffer;
            free(buffer); //free up memory allocation
        }
        http_free(id); //always clean up the id!

        if (Logger) {
            Logger->logTrace("<-- %s\n", ss.str().c_str());
        }

        Response<T> response;
        response.parseContent<CallerT>(ss.str());
        return response;
    }

} // namespace alpaca
