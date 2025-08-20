#pragma once

#include <sstream>
#include <string>
#include <cassert>
#include <type_traits>
#include <thread>
#include "alpaca/json.h"
#include "logger.h"
#include "throttler.h"
#include "rapidjson/error/en.h"

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
    extern std::unique_ptr<Throttler> s_throttler;
    extern uint64_t get_timestamp();

    template<typename>
    struct is_vector : std::false_type {};

    template<typename T, typename A>
    struct is_vector<std::vector<T, A>> : std::true_type {};

    enum AssetType : uint8_t {
        USSecurity,
        Crypto,
        Option
    };

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
        template<typename T>
        friend Response<T> request(const std::string&, const char*, const char*, spdlog::level::level_enum logLevel, uint64_t timestamp);

        template<typename T>
        friend void request(Response<T>& response, const std::string&, const char*, const char*, spdlog::level::level_enum logLevel, uint64_t timestamp);

        bool parseContent(const std::string& content, const std::string& url) {
            rapidjson::Document d;
            if (d.Parse(content.c_str()).HasParseError()) {
                SPDLOG_ERROR("Received parse error when deserializing asset JSON. err={} error_offset={} content={}", GetParseError_En(d.GetParseError()), d.GetErrorOffset(), content);
                onError("Received parse error when deserializing asset JSON. err=" + std::to_string(d.GetParseError()) + "\n" + content);
                return false;
            }

            if (!d.IsObject()) {
                if (d.IsArray()) {
                    if (!is_vector<T>::value) {
                        onError("JSON is an arry type, but the response content is an object.");
                        return false;
                    }
                }
                else {
                    message_ = "Deserialized valid JSON but it wasn't an object";
                    code_ = 1;
                    return false;
                }
            }
            else {
                if (d.HasMember("code") && d.HasMember("message")) {
                    auto code = d["code"].GetInt();
                    if (code == 42910000)
                    {
                        s_throttler->enableThrottle(true);
                    }
                    onError(d["message"].GetString(), code);
                    SPDLOG_ERROR("<-- {}", content);
                    return true;
                }
                else if (!d.HasMember("code") && d.HasMember("message") /*&& (strcmp(d["message"].GetString(), "too many requests.") == 0)*/) {
                    onError(d["message"].GetString());
                    SPDLOG_ERROR("<-- {}", content);
                    return true;
                }
            }

            try {
                Parser<rapidjson::Document> parser(d);
                auto result = parse<T>(parser, content_);
                onError(result.second, result.first);   // This might not be an error, just to set the meassage and code
            }
            catch (std::exception& e) {
                onError(e.what());
            }
            return false;
        }
        
        template<typename U>
        std::pair<int, std::string> parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<!is_vector<U>::value>::type* = 0) {
            return content.fromJSON(parser);
        }

        template<typename U>
        std::pair<int, std::string> parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<is_vector<U>::value>::type* = 0) {
            auto parseArray = [&](auto& arrayObj) -> std::pair<int, std::string> {
                for (auto& item : arrayObj.GetArray()) {
                    if (!item.IsObject()) {
                        assert(false);
                        continue;
                    }
                    auto objJson = item.GetObject();
                    Parser<decltype(item.GetObject())> itemParser(objJson);
                    typename U::value_type obj;
                    obj.fromJSON(itemParser);
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
    template<typename T>
    inline void request(Response<T>& response, const std::string& url, const char* headers = nullptr, const char* data = nullptr, spdlog::level::level_enum logLevel = spdlog::level::trace, uint64_t timestamp = get_timestamp()) {
        if (url.empty()) {
            return response.onError("Invalid url - empty");
        }

        if (!Config::get().alpacaPaidPlan && !s_throttler->waitForSending(timestamp)) {
            // reached throttle limit
            return response.onError("Brokerprogress returned zero. Aborting...");
        }

        spdlog::log(logLevel, "--> {}", url);

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
        
        bool error_msg_logged = response.parseContent(ss.str(), url);
        if (!error_msg_logged) {
            spdlog::log(logLevel, "<-- {}", ss.str());
        }
    }

    template<typename T>
    inline Response<T> request(const std::string& url, const char* headers = nullptr, const char* data = nullptr, spdlog::level::level_enum logLevel = spdlog::level::trace, uint64_t timestamp = get_timestamp()) {
        Response<T> response;
        request<T>(response, url, headers, data, logLevel, timestamp);
        return response;
    }

} // namespace alpaca
