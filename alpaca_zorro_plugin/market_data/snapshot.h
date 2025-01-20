#pragma once

#include <string>
#include "market_data/trade.h"
#include "market_data/quote.h"

#ifdef _WIN32
// Remove GetObject definition from windows.h, which prevents calls to
// RapidJSON's GetObject.
// https://github.com/Tencent/rapidjson/issues/1448
#undef GetObject
#endif  // _WIN32

namespace alpaca {

    struct Snapshot {
		std::string symbol;

		Trade last_trade;
		Quote last_quote;

	private:
		template<typename> friend class Response;

		template<typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			parser.get<std::string>("symbol", symbol);

			if (parser.json.HasMember("latestTrade") && parser.json["latestTrade"].IsObject()) {
				auto obj = parser.json["latestTrade"].GetObject();
				Parser<decltype(obj)> p(obj);
				last_trade.fromJSON(p);
			}

			if (parser.json.HasMember("latestQuote") && parser.json["latestQuote"].IsObject()) {
				auto obj = parser.json["latestQuote"].GetObject();
				Parser<decltype(obj)> p(obj);
				last_quote.fromJSON(p);
			}
			return std::make_pair(0, "OK");
		}
    };
}