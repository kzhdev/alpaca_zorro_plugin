#include "rapidjson/document.h"
#include <vector>

namespace alpaca {
    
    template<typename T>
    struct Parser {
        const T& json;

        Parser(const T& j) : json(j) {}

        template<typename U>
        bool get(const char* name, U& value) const;

        template<>
        bool get<std::string>(const char* name, std::string& value) const {
            if (json.HasMember(name) && json[name].IsString()) {
                value = json[name].GetString();
                return true;
            }
            return false;
        }

        template<>
        bool get<int32_t>(const char* name, int32_t& value) const {
            if (json.HasMember(name)) {
                if (json[name].IsInt()) {
                    value = json[name].GetInt();
                    return true;
                }
                if (json[name].IsUint()) {
                    value = json[name].GetUint();
                    return true;
                }
                if (json[name].IsString()) {
                    value = atoi(json[name].GetString());
                    return true;
                }
            }
            return false;
        }

        template<>
        bool get<uint32_t>(const char* name, uint32_t& value) const {
            if (json.HasMember(name)) {
                if (json[name].IsUint()) {
                    value = json[name].GetUint();
                    return true;
                }
                if (json[name].IsInt()) {
                    value = json[name].GetInt();
                    return true;
                }
                if (json[name].IsString()) {
                    value = atoi(json[name].GetString());
                }
            }
            return false;
        }

        template<>
        bool get<int64_t>(const char* name, int64_t& value) const {
            if (json.HasMember(name)) {
                if (json[name].IsInt64()) {
                    value = json[name].GetInt64();
                    return true;
                }
                if (json[name].IsUint64()) {
                    value = json[name].IsUint64();
                    return true;
                }
                if (json[name].IsString()) {
                    value = _atoi64(json[name].GetString());
                    return true;
                }
            }
            return false;
        }

        template<>
        bool get<uint64_t>(const char* name, uint64_t& value) const {
            if (json.HasMember(name)) {
                if (json[name].IsUint64()) {
                    value = json[name].IsUint64();
                    return true;
                }
                if (json[name].IsInt64()) {
                    value = json[name].GetInt64();
                    return true;
                }
                if (json[name].IsString()) {
                    value = _atoi64(json[name].GetString());
                    return true;
                }
            }
            return false;
        }

        template<>
        bool get<bool>(const char* name, bool& value) const {
            if (json.HasMember(name) && json[name].IsBool()) {
                value = json[name].GetBool();
                return true;
            }
            return false;
        }

        template<>
        bool get<double>(const char* name, double& value) const {
            if (json.HasMember(name)) {
                if (json[name].IsNumber()) {
                    value = json[name].GetDouble();
                    return true;
                }
                if (json[name].IsString()) {
                    value = atof(json[name].GetString());
                    return true;
                }
            }
            return false;
        }

        template<>
        bool get<float>(const char* name, float& value) const {
            if (json.HasMember(name)) {
                if (json[name].IsNumber()) {
                    value = json[name].GetFloat();
                    return true;
                }
                if (json[name].IsString()) {
                    value = (float)atof(json[name].GetString());
                    return true;
                }
            }
            return false;
        }

        template<>
        bool get<std::vector<double>>(const char* name, std::vector<double>& value) const {
            if (json.HasMember(name) && json[name].IsArray()) {
                for (auto& item : json[name].GetArray()) {
                    if (item.IsNumber()) {
                        value.push_back(item.GetDouble());
                    }
                }
                return true;
            }
            return false;
        }

        template<>
        bool get<std::vector<float>>(const char* name, std::vector<float>& value) const {
            if (json.HasMember(name) && json[name].IsArray()) {

                for (auto& item : json[name].GetArray()) {
                    if (item.IsNumber()) {
                        value.push_back(item.GetFloat());
                    }
                }
                return true;
            }
            return false;
        }

        template<>
        bool get<std::vector<uint64_t>>(const char* name, std::vector<uint64_t>& value) const {
            if (json.HasMember(name) && json[name].IsArray()) {

                for (auto& item : json[name].GetArray()) {
                    if (item.IsNumber()) {
                        value.push_back(item.GetUint64());
                    }
                }
                return true;
            }
            return false;
        }

        template<typename U>
        U get(const char* name) const {
            if (json.HasMember(name) && json[name].IsString()) {
                return json[name].GetString();
            }
            return "";
        }
    };

    

   /* using JsonT = rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>;
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
    }*/
}
