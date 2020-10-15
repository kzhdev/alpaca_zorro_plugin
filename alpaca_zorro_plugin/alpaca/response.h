#pragma once

#include <sstream>
#include <string>
#include <cassert>
#include <type_traits>
#include "json.h"

namespace alpaca {

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
        friend class Client;
        void parseContent(const std::string& content) {
            rapidjson::Document d;
            if (d.Parse(content.c_str()).HasParseError()) {
                message_ = "Received parse error when deserializing asset JSON. err=" + d.GetParseError();
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
            else if (d.HasMember("code") && d.HasMember("message")) {
                message_ = d["message"].GetString();
                code_ = d["code"].GetInt();
                return;
            }
            else if (!d.HasMember("code") && d.HasMember("message") && (strcmp(d["message"].GetString(), "too many requests.") == 0)) {
                message_ = d["message"].GetString();
                code_ = 1;
                return;
            }

            try {
                Parser<rapidjson::Document> parser(d);
                parse(parser, content_);
            }
            catch (std::exception& e) {
                code_ = 1;
                message_ = e.what();
            }
        }
        
        template<typename U>
        void parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<!is_vector<U>::value>::type* = 0) {
            content.fromJSON(parser);
        }

        template<typename U>
        void parse(Parser<rapidjson::Document>& parser, U& content, typename std::enable_if<is_vector<U>::value>::type* = 0) {
            assert(parser.json.IsArray());
            for (auto& item : parser.json.GetArray()) {
                if (!item.IsObject()) {
                    assert(false);
                    continue;
                }
                Parser<decltype(item.GetObject())> itemParser(item.GetObject());
                U::value_type obj;
                obj.fromJSON(itemParser);
                content.emplace_back(std::move(obj));
            }
        }

    private:
        int code_;
        std::string message_;
        T content_;
    };
} // namespace alpaca
