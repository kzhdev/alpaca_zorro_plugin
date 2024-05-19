#pragma once

#include <map>
#include <string>
#include <vector>

#ifdef _WIN32
// Remove GetObject definition from windows.h, which prevents calls to
// RapidJSON's GetObject.
// https://github.com/Tencent/rapidjson/issues/1448
#undef GetObject
#endif  // _WIN32

namespace alpaca {
	std::string timeToString(__time32_t time);

	enum class Adjustment : uint8_t
	{
		raw = 0,
		split = 1,
		devidend = 2,
		all = 3,
	};

	inline const char* to_string(Adjustment adj)
	{
		constexpr const char* str_adj[] = {
			"raw", "split", "devidend", "all"
		};
		return str_adj[static_cast<uint8_t>(adj)];
	}

	struct Bar {
		std::string time_string;
		uint32_t time;
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		uint32_t volume;

	private:
		template<typename> friend class Response;
		friend class Bars;

		template<typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser, typename std::enable_if<std::is_same<CallerT, class AlpacaMarketData>::value>::type* = 0) {
			parser.get<std::string>("t", time_string);
			time = parseTimeStamp2(time_string);
			parser.get<double>("o", open_price);
			parser.get<double>("h", high_price);
			parser.get<double>("l", low_price);
			parser.get<double>("c", close_price);
			parser.get<uint32_t>("v", volume);
			return std::make_pair(0, "OK");
		}

		template<typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser, typename std::enable_if<std::is_same<CallerT, class Polygon>::value>::type* = 0) {
			uint64_t t;
			parser.get<uint64_t>("t", t);
			time = static_cast<uint32_t>(t / 1000);
			parser.get<double>("o", open_price);
			parser.get<double>("h", high_price);
			parser.get<double>("l", low_price);
			parser.get<double>("c", close_price);
			parser.get<uint32_t>("v", volume);
			return std::make_pair(0, "OK");
		}
	};

	/**
		* @brief A type representing bars for multiple symbols
		*/
	class Bars {
	public:
		std::string next_page_token;
		std::vector<Bar> bars;

	private:
		template<typename> friend class Response;

		template<typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			parser.get<std::string>("next_page_token", next_page_token);
			if (parser.json.HasMember("bars")) {
				if (parser.json["bars"].IsNull()) {
					return std::make_pair(0, "OK");
				}
				for (auto& symbol_bar : parser.json["bars"].GetArray()) {
					auto barJson = symbol_bar.GetObject();
					Parser<decltype(symbol_bar.GetObject())> p(barJson);
					Bar bar;
					bar.fromJSON<CallerT>(p);
					bars.emplace_back(std::move(bar));
				}
			}
			return std::make_pair(0, "OK");
		}
	};
} // namespace alpaca
