#include "rapidjson/document.h"
#include <vector>

namespace alpaca {
    using JsonT = rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>;
    template<typename T>
    inline T parse(const JsonT& d, const char* name);

    template<>
    inline std::string parse<std::string>(const JsonT& d, const char* name) {
        if (d.HasMember(name) && d[name].IsString()) {
            return d[name].GetString();
        }
        return "";
    }

    template<>
    inline int32_t parse<int32_t>(const JsonT& d, const char* name) {
        if (d.HasMember(name)) {
            if (d[name].IsInt()) {
                return d[name].GetInt();
            }
            if (d[name].IsUint()) {
                return d[name].GetUint();
            }
            if (d[name].IsString()) {
                return atoi(d[name].GetString());
            }
        }
        return 0;
    }

    template<>
    inline uint32_t parse<uint32_t>(const JsonT& d, const char* name) {
        if (d.HasMember(name)) {
            if (d[name].IsUint()) {
                return d[name].GetUint();
            }
            if (d[name].IsInt()) {
                return d[name].GetInt();
            }
            if (d[name].IsString()) {
                return atoi(d[name].GetString());
            }
        }
        return 0;
    }

    template<>
    inline int64_t parse<int64_t>(const JsonT& d, const char* name) {
        if (d.HasMember(name)) {
            if (d[name].IsInt64()) {
                return d[name].GetInt64();
            }
            if (d[name].IsUint64()) {
                return d[name].IsUint64();
            }
            if (d[name].IsString()) {
                return _atoi64(d[name].GetString());
            }
        }
        return 0;
    }

    template<>
    inline uint64_t parse<uint64_t>(const JsonT& d, const char* name) {
        if (d.HasMember(name)) {
            if (d[name].IsUint64()) {
                return d[name].IsUint64();
            }
            if (d[name].IsInt64()) {
                return d[name].GetInt64();
            }
            if (d[name].IsString()) {
                return _atoi64(d[name].GetString());
            }
        }
        return 0;
    }

    template<>
    inline bool parse<bool>(const JsonT& d, const char* name) {
        if (d.HasMember(name) && d[name].IsBool()) {
            return d[name].GetBool();
        }
        return false;
    }

    template<>
    inline double parse<double>(const JsonT& d, const char* name) {
        if (d.HasMember(name)) {
            if (d[name].IsNumber()) {
                return d[name].GetDouble();
            }
            if (d[name].IsString()) {
                return atof(d[name].GetString());
            }
        }
        return 0.;
    }

    template<>
    inline float parse<float>(const JsonT& d, const char* name) {
        if (d.HasMember(name)) {
            if (d[name].IsNumber()) {
                return d[name].GetFloat();
            }
            if (d[name].IsString()) {
                return (float)atof(d[name].GetString());
            }
        }
        return 0.;
    }

    template<>
    inline std::vector<double> parse<std::vector<double>>(const JsonT& d, const char* name) {
        std::vector<double> items;
        if (d.HasMember(name) && d[name].IsArray()) {

            for (auto& item : d[name].GetArray()) {
                if (item.IsNumber()) {
                    items.push_back(item.GetDouble());
                }
            }
        }
        return items;
    }

    template<>
    inline std::vector<float> parse<std::vector<float>>(const JsonT& d, const char* name) {
        std::vector<float> items;
        if (d.HasMember(name) && d[name].IsArray()) {

            for (auto& item : d[name].GetArray()) {
                if (item.IsNumber()) {
                    items.push_back(item.GetFloat());
                }
            }
        }
        return items;
    }

    template<>
    inline std::vector<uint64_t> parse<std::vector<uint64_t>>(const JsonT& d, const char* name) {
        std::vector<uint64_t> items;
        if (d.HasMember(name) && d[name].IsArray()) {

            for (auto& item : d[name].GetArray()) {
                if (item.IsNumber()) {
                    items.push_back(item.GetUint64());
                }
            }
        }
        return items;
    }
}
