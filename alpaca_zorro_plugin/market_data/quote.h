#pragma once

#include <string>

namespace alpaca {

	struct Quote {
		double ask_price;
		int ask_size;
		int ask_exchange;
		double bid_price;
		int bid_size;
		int bid_exchange;
		uint64_t timestamp;
		std::string sTime;

	private:
		template<typename> friend class Response;
		friend struct LastQuote;
		friend struct Quotes;

		template<typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			if (parser.json.HasMember("timestamp")) {
				parser.get<double>("askprice", ask_price);
				parser.get<int>("asksize", ask_size);
				parser.get<int>("askexchange", ask_exchange);
				parser.get<double>("bidprice", bid_price);
				parser.get<int>("bidsize", bid_size);
				parser.get<int>("bidexchange", bid_exchange);
				parser.get<uint64_t>("timestamp", timestamp);
			}
			else {
				// TODO: Fix exchange parsing
				parser.get<double>("ap", ask_price);
				parser.get<int>("as", ask_size);
				parser.get<int>("ax", ask_exchange);
				parser.get<double>("bp", bid_price);
				parser.get<int>("bs", bid_size);
				parser.get<int>("bx", bid_exchange);
				parser.get<std::string>("t", sTime);
				timestamp = parseTimeStamp2(sTime);
			}
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
			parser.get<std::string>("status", status);
			parser.get<std::string>("symbol", symbol);

			if (status != "success") {
				return std::make_pair(1, symbol + " " + status);
			}

			
			if (parser.json.HasMember("last") && parser.json["last"].IsObject()) {
				auto obj = parser.json["last"].GetObject();
				Parser<decltype(parser.json["last"].GetObject())> p(obj);
				quote.fromJSON(p);
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
					t.fromJSON(p);
					quotes.emplace_back(std::move(t));
				}
			}
			return std::make_pair(0, "OK");
		}
	};
} // namespace alpaca