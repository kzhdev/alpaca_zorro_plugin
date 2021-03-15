#pragma once

#include <cstdint>
#include <string>
#include <bitset>

namespace {
    inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    }

    inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    }

    inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }
}

namespace alpaca {

    struct Configuration {
        std::string polygonApiKey;
        uint8_t dataSource = 0;
        uint8_t logLevel = 0;
        bool alpacaPaidPlan = false;
        bool fillDelayedDataWithPolygon = 0;

        void init() {
            if (!readConfig("./ZorroFix.ini")) {
                readConfig("./Zorro.ini");
            }
        }

    private:
        inline bool readConfig(const char* file) {
            std::ifstream config(file);
            if (!config.is_open()) {
                return false;
            }

            std::string line;

            while (getline(config, line)) {
                getConfig(line, ConfigFound::DataSource, "AlpacaDataSource", dataSource);
                getConfig(line, ConfigFound::PaidPlan,  "AlpacaPaidDataPlan", alpacaPaidPlan);
                getConfig(line, ConfigFound::LogLevel, "AlpacaLevelLevel", logLevel);
                getConfig(line, ConfigFound::PolygonApiKey, "PolygonApiKey", polygonApiKey);
                getConfig(line, ConfigFound::FillDelayedDataWithPolygon, "AlpacaFillDelayedDataWithPolygon", fillDelayedDataWithPolygon);
            }

            return configFound_.all();
        }

        template<typename T>
        inline void getConfig(std::string& line, uint8_t foundFlag, const char* configName, T& value) {
            ltrim(line);
            if (line.find("//") == 0) {
                return;
            }

            if (configFound_.test(foundFlag)) {
                return;
            }

            auto pos = line.find(configName);
            if (pos == std::string::npos) {
                return;
            }
            auto pos1 = line.find("=");
            if (pos1 == std::string::npos) {
                return;
            }

            std::string v = line.substr(pos1 + 1);
            pos1 = v.rfind("//");
            if (pos1 != std::string::npos) {
                v = v.substr(0, pos1);
            }

            trim(v);

            setValue(value, v);
            configFound_.set(foundFlag, 1);
        }

        template<typename T>
        inline typename std::enable_if<std::is_same<T, std::string>::value>::type setValue(T& value, const std::string& v) { 
            value = v;
            // remove ""
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            //value = value.substr(1);
            //value = value.substr(0, value.size() - 1);
        }

        template<typename T>
        inline typename std::enable_if<!std::is_same<T, std::string>::value>::type setValue(T& value, const std::string& v) { value = atoi(v.c_str()); }

    private:
        enum ConfigFound : uint8_t {
            DataSource,
            PaidPlan,
            PolygonApiKey,
            FillDelayedDataWithPolygon,
            LogLevel,
        };
        std::bitset<8> configFound_ = 0;
    };
}