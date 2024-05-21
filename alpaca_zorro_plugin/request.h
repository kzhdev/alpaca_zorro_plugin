#pragma once

#include <sstream>
#include <string>
#include <cassert>
#include <type_traits>
#include <thread>
#include "alpaca/json.h"
#include "logger.h"

#ifdef _WIN32
// Remove GetObject definition from windows.h, which prevents calls to
// RapidJSON's GetObject.
// https://github.com/Tencent/rapidjson/issues/1448
#undef GetObject
#endif  // _WIN32

namespace alpaca {

    extern int(__cdecl* BrokerError)(const char* txt);
    extern int(__cdecl* BrokerProgress)(const int percent);
    extern int(__cdecl* http_send)(char* url, char* data, char* header);
    extern long(__cdecl* http_status)(int id);
    extern long(__cdecl* http_result)(int id, char* content, long size);
    extern void(__cdecl* http_free)(int id);
    extern uint64_t get_timestamp();

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
        Response(int c, std::string m) noexcept : code_(c), message_(m) {}

        void onError(std::string m, int c = 1) {
            code_ = c;
            message_ = std::move(m);
            timestamp_ = get_timestamp();
        }

        int getCode() const noexcept {
            return code_;
        }

        uint64_t timestamp() const noexcept {
            return timestamp_;
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

        void reset(int c, std::string m) noexcept {
            message_ = std::move(m);
            code_ = c;
            timestamp_ = 0;
        }

    private:
        template<typename T, typename CallerT>
        friend Response<T> request(const std::string&, const char*, const char*, LogLevel logLevel, LogType type);

        template<typename T, typename CallerT>
        friend void request(Response<T>& response, const std::string&, const char*, const char*, LogLevel logLevel, LogType type);

        template<typename CallerT>
        void parseContent(const std::string& content, const std::string& url) {
            rapidjson::Document d;
            if (d.Parse(content.c_str()).HasParseError()) {
                onError("Received parse error when deserializing asset JSON. err=" + std::to_string(d.GetParseError()) + "\n" + content);
                return;
            }

            if (!d.IsObject()) {
                if (d.IsArray()) {
                    if (!is_vector<T>::value) {
                        onError("JSON is an arry type, but the response content is an object.");
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
                    onError(d["message"].GetString());
                    return;
                }
                else if (!d.HasMember("code") && d.HasMember("message") /*&& (strcmp(d["message"].GetString(), "too many requests.") == 0)*/) {
                    onError(d["message"].GetString());
                    return;
                }
            }
            else {
                // Check polygon return
                if (d.HasMember("error") && d.HasMember("errorcode")) {
                    std::string msg = d["error"].GetString();
                    msg.append(" ").append(url);
                    onError(std::move(msg), atoi(d["errorcode"].GetString()));
                    return;
                }
            }

            try {
                Parser<rapidjson::Document> parser(d);
                auto result = parse<T, CallerT>(parser, content_);
                onError(result.second, result.first);   // This might not be an error, just to set the meassage and code
            }
            catch (std::exception& e) {
                onError(e.what());
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
        uint64_t timestamp_ = 0;
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
    inline void request(Response<T>& response, const std::string& url, const char* headers = nullptr, const char* data = nullptr, LogLevel logLevel = LogLevel::L_TRACE, LogType type = LogType::LT_ALL) {
        if (url.empty()) {
            return response.onError("Invalid url - empty");
        }

       LOG_DEBUG("--> %s\n", url.c_str());

        int id = http_send((char*)url.c_str(), (char*)data, (char*)headers);

        if (!id) {
            return response.onError("Cannot connect to server");
        }

        long n = 0;
        std::stringstream ss;
        while (!(n = http_status(id))) {
            if (!BrokerProgress(1)) {
                http_free(id);
                return response.onError("Brokerprogress returned zero. Aborting...");
            }
            std::this_thread::yield();
            //using namespace std::chrono_literals;
            //std::this_thread::sleep_for(100us);
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
                return response.onError("Id is invalid", n);
            case -3:
                return response.onError("Website did not response", n);
            case -4:
                return response.onError("Host could not be resolved", n);
            default:
                return response.onError("Transfer Failed", n);
            }
        }

        Logger::instance().log(logLevel, type, "<-- %s\n", ss.str().c_str());

        return response.parseContent<CallerT>(ss.str(), url);
    }

    template<typename T, typename CallerT>
    inline Response<T> request(const std::string& url, const char* headers = nullptr, const char* data = nullptr, LogLevel logLevel = LogLevel::L_TRACE, LogType type = LogType::LT_ALL) {
        Response<T> response;
        request<T, CallerT>(response, url, headers, data, logLevel, type);
        return response;
    }

} // namespace alpaca
