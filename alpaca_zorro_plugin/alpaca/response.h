#pragma once

#include <sstream>
#include <string>
#include <cassert>

#include "json.h"

namespace alpaca {

    /**
     * @brief The status of various Alpaca actions.
     */
    enum ActionStatus {
        Open,
        Closed,
        Active,
        All,
    };

    /**
     * @brief A helper to convert an ActionStatus to a string
     */
    constexpr const char* to_string(ActionStatus status) {
        constexpr const char* sActionStatus[] = { "Open", "Closed", "Active", "All" };
        assert(status >= ActionStatus::Open && status <= ActionStatus::All);
        return sActionStatus[status];
    }

    template<typename T>
    class Response {
    public:
        /**
         * @brief Default constructor
         *
         * Note that the default constructor initialized an alpaca::Status instance
         * to a state such that a successful operation is indicated.
         */
        explicit Response(int c = 0) : code_(c), message_("OK") {}

        /**
         * @brief A constructor which can be used to concisely express the status of
         * an operation.
         *
         * @param c a status code. The idiom is that a zero status code indicates a
         * successful operation and a non-zero status code indicates a failed
         * operation.
         * @param m a message indicating some extra detail regarding the operation.
         * If all operations were successful, this message should be "OK".
         * Otherwise, it doesn't matter what the string is, as long as both the
         * setter and caller agree.
         */
        Response(int c, std::string m) : code_(c), message_(std::move(m)) {}

    public:
        /**
         * @brief A getter for the status code property
         *
         * @return an integer representing the status code of the operation.
         */
        int getCode() const {
            return code_;
        }

        /**
         * @brief A getter for the message property
         *
         * @return a string representing arbitrary additional information about the
         * success or failure of an operation. On successful operations, the idiom
         * is for the message to be "OK"
         */
        std::string getMessage() const noexcept {
            return message_;
        }

        /**
         * @brief A convenience method to check if the return code is 0
         *
         * @code{.cpp}
         *   auto s = doSomething();
         *   if (s.ok()) {
         *     LOG(INFO) << "doing work";
         *   } else {
         *     LOG(ERROR) << s.toString();
         *   }
         * @endcode
         *
         * @return a boolean which is true if the status code is 0, false otherwise.
         */
        bool ok() const noexcept {
            return getCode() == 0;
        }

        /**
         * @brief A synonym for alpaca::Status::getMessage()
         *
         * @see getMessage()
         */
        std::string what() const noexcept {
            return getMessage();
        }

        T& content() noexcept {
            return content_;
        }

        /**
         * @brief implicit conversion to bool
         *
         * Allows easy use of Status in an if statement, as below:
         *
         * @code{.cpp}
         *   if (doSomethingThatReturnsStatus()) {
         *     LOG(INFO) << "Success!";
         *   }
         * @endcode
         */
        explicit operator bool() const {
            return ok();
        }

    private:
        friend class Client;
        void parseContent(const std::string& content) {
            rapidjson::Document d;
            if (d.Parse(content.c_str()).HasParseError()) {
                message_ = "Received parse error when deserializing asset JSON";
                code_ = 1;
                return;
            }

            if (!d.IsObject()) {
                message_ = "Deserialized valid JSON but it wasn't an object";
                code_ = 1;
                return;
            }

            if (d.HasMember("code") && d.HasMember("message")) {
                message_ = d["message"].GetString();
                code_ = d["code"].GetInt();
                return;
            }

            try {
                Parser<rapidjson::Document> parser(d);
                content_.fromJSON(parser);
            }
            catch (std::exception& e) {
                code_ = 1;
                message_ = e.what();
            }
        }

    private:
        int code_;
        std::string message_;
        T content_;
    };
} // namespace alpaca
