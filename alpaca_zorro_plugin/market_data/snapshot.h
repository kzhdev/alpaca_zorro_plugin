#pragma once

#include <string>
#include "market_data/trade.h"
#include "market_data/quote.h"

namespace alpaca {

    struct Snapshot {
		std::string symbol;

		Trade last_trade;
		Quote last_quote;

	private:
		template<typename> friend class Response;

		template<typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			parser.get<std::string>("symbol", symbol);

			if (parser.json.HasMember("latestTrade") && parser.json["latestTrade"].IsObject()) {
				auto obj = parser.json["latestTrade"].GetObject();
				Parser<decltype(obj)> p(obj);
				last_trade.fromJSON<CallerT>(p);
			}

			if (parser.json.HasMember("latestQuote") && parser.json["latestQuote"].IsObject()) {
				auto obj = parser.json["latestQuote"].GetObject();
				Parser<decltype(obj)> p(obj);
				last_quote.fromJSON<CallerT>(p);
			}
			return std::make_pair(0, "OK");
		}
    };
}