#pragma once

#include <cstdint>
#include <string>
#include <bitset>
#include <market_data/bars.h>
#include <fstream>

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

    struct Config {
        double fractionalLotAmount = 1;
        uint8_t logLevel = 0;
        bool alpacaPaidPlan = true;
        bool useWebsocket = true;
        bool passLogLevelToWebsocketProxy = false;
        Adjustment adjustment = Adjustment::all;

        static Config& get()
        {
            static Config instance;
            return instance;
        }

        Config(const Config&) = delete;
        Config(Config&&) = delete;
        Config& operator=(const Config&) = delete;
        Config& operator=(Config&&) = delete;

    private:
        Config() {
            readConfig("./Zorro.ini");
            readConfig("./ZorroFix.ini");
        }
        ~Config() = default;

        inline bool readConfig(const char* file) {
            std::ifstream config(file);
            if (!config.is_open()) {
                return false;
            }

            std::string line;

            while (getline(config, line)) {
                getConfig(line, ConfigFound::PaidPlan,  "AlpacaPaidDataPlan", alpacaPaidPlan);
                getConfig(line, ConfigFound::LogLevel, "AlpacaLogLevel", logLevel);
                getConfig(line, ConfigFound::UseWebsocket, "AlpacaUseWebsocket", useWebsocket);
                getConfig(line, ConfigFound::FractionalLotAmount, "AlpacaFractionalLotAmount", fractionalLotAmount);
                getConfig(line, ConfigFound::PassLogLevelToWebsocketProxy, "AlpacaPassLogLevelToWebsocketProxy", passLogLevelToWebsocketProxy);
            }

            return configFound_.all();
        }

        template<typename T>
        inline void getConfig(std::string& line, uint8_t foundFlag, const char* configName, T& value) {
            ltrim(line);
            if (line.find("//") == 0) {
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
        }

        template<typename T>
        inline typename std::enable_if<std::is_integral<T>::value>::type setValue(T& value, const std::string& v) { value = atoi(v.c_str()); }

        template<typename T>
        inline typename std::enable_if<std::is_floating_point<T>::value>::type setValue(T& value, const std::string& v) { value = atof(v.c_str()); }


    private:
        enum ConfigFound : uint8_t {
            PaidPlan,
            LogLevel,
            UseWebsocket,
            FractionalLotAmount,
            PassLogLevelToWebsocketProxy,
            __count__,  // for internal use only
        };
        std::bitset<ConfigFound::__count__> configFound_ = 0;
    };
}