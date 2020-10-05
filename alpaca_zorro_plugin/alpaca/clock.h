#pragma once

#include <string>
#include "rapidjson/document.h"

namespace alpaca {

    struct Clock {
        __time32_t next_close;
        __time32_t next_open;
        __time32_t timestamp;
        bool is_open;

    private:
        template<typename> friend class Response;
        void fromJSON(const rapidjson::Document& d);
    };
} // namespace alpaca