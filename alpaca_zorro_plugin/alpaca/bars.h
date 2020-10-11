#pragma once

#include <map>
#include <string>
#include <vector>

namespace alpaca {

	std::string timeToString(__time32_t time);

	struct Bar {
		uint32_t time;
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		uint32_t volume;

	private:
		template<typename> friend class Response;
		friend class Bars;

		template<typename T>
		void fromJSON(const T& parser) {
			parser.get<uint32_t>("t", time);
			parser.get<double>("o", open_price);
			parser.get<double>("h", high_price);
			parser.get<double>("l", low_price);
			parser.get<double>("c", close_price);
			parser.get<uint32_t>("v", volume);
		}
	};

	/**
	 * @brief A type representing bars for multiple symbols
	 */
	class Bars {
	public:
		std::map<std::string, std::vector<Bar>> bars;

	private:
		template<typename> friend class Response;

		template<typename T>
		void fromJSON(const T& parser) {
			for (auto symbol_bars = parser.json.MemberBegin(); symbol_bars != parser.json.MemberEnd(); symbol_bars++) {
				bars[symbol_bars->name.GetString()] = std::vector<Bar>{};
				for (auto& symbol_bar : symbol_bars->value.GetArray()) {
					Bar bar;
					Parser<decltype(symbol_bar.GetObject())> parser(symbol_bar.GetObject());
					bar.fromJSON(parser);
					bars[symbol_bars->name.GetString()].emplace_back(std::move(bar));
				}
			}
		}
	};
} // namespace alpaca
