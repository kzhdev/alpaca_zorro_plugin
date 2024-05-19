#pragma once

#include <string>
#include <unordered_map>

#ifdef _WIN32
// Remove GetObject definition from windows.h, which prevents calls to
// RapidJSON's GetObject.
// https://github.com/Tencent/rapidjson/issues/1448
#undef GetObject
#endif  // _WIN32

namespace alpaca {

	struct Quote {
		double ask_price;
		int ask_size;
		//int ask_exchange;
		double bid_price;
		int bid_size;
		//int bid_exchange;
		uint64_t timestamp;
		std::string sTime;

	private:
		template<typename> friend class Response;
		friend struct LastQuote;
		friend struct Quotes;
		friend struct Snapshot;
		friend struct LastQuotes;

		template<typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser, typename std::enable_if<std::is_same<CallerT, class AlpacaMarketData>::value>::type* = 0) {
			// TODO: Fix exchange parsing
			parser.get<double>("ap", ask_price);
			parser.get<int>("as", ask_size);
			parser.get<double>("bp", bid_price);
			parser.get<int>("bs", bid_size);
			parser.get<std::string>("t", sTime);
			timestamp = parseTimeStamp2(sTime);
			return std::make_pair(0, "OK");
		}

		template<typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser, typename std::enable_if<std::is_same<CallerT, class Polygon>::value>::type* = 0) {
			parser.get<double>("askprice", ask_price);
			parser.get<int>("asksize", ask_size);
			//parser.get<int>("askexchange", ask_exchange);
			parser.get<double>("bidprice", bid_price);
			parser.get<int>("bidsize", bid_size);
			//parser.get<int>("bidexchange", bid_exchange);
			parser.get<uint64_t>("timestamp", timestamp);
			return std::make_pair(0, "OK");
		}
	};

	struct LastQuote {
		std::string status;
		std::string symbol;
		Quote quote;

	private:
		template<typename> friend class Response;
		
		template<typename CallerT, typename T> 
		std::pair<int, std::string> fromJSON(const T& parser/*, typename std::enable_if<std::is_same<CallerT, class AlpacaMarketData>::value>::type* = 0*/) {
			parser.get<std::string>("symbol", symbol);

			if (parser.json.HasMember("quote") && parser.json["quote"].IsObject()) {
				auto obj = parser.json["quote"].GetObject();
				Parser<decltype(parser.json["quote"].GetObject())> p(obj);
				quote.fromJSON<CallerT>(p);
			}
			return std::make_pair(0, "OK");
		}
	};

	struct Quotes {
		std::string symbol;
		std::string next_page_token;
		std::vector<Quote> quotes;

	private:
		template <typename> friend class Response;

		template <typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			parser.get<std::string>("symbol", symbol);

			parser.get<std::string>("next_page_token", next_page_token);
			if (parser.json.HasMember("quotes")) {
				for (auto& trade : parser.json["quotes"].GetArray()) {
					auto tradeJson = trade.GetObject();
					Parser<decltype(tradeJson)> p(tradeJson);
					Quote t;
					t.fromJSON<CallerT>(p);
					quotes.emplace_back(std::move(t));
				}
			}
			return std::make_pair(0, "OK");
		}
	};

	struct LastQuotes {
		std::unordered_map<std::string, Quote> quotes;

	private:
		template <typename> friend class Response;

		template <typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			if (parser.json.HasMember("quotes")) {
				for (const auto& quoteJson : parser.json["quotes"].GetObject()) {
					auto quoteObj = quoteJson.value.GetObject();
					Parser<decltype(quoteObj)> p(quoteObj);
					Quote quote;
					quote.fromJSON<CallerT>(p);
					quotes.emplace(quoteJson.name.GetString(), std::move(quote));
				}
			}
			return std::make_pair(0, "OK");
		}
	};
} // namespace alpaca