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

	private:
		template<typename> friend class Response;
		friend struct LastQuote;

		template<typename T>
		void fromJSON(const T& parser) {
			parser.get<double>("askprice", ask_price);
			parser.get<int>("asksize", ask_size);
			parser.get<int>("askexchange", ask_exchange);
			parser.get<double>("bidprice", bid_price);
			parser.get<int>("bidsize", bid_size);
			parser.get<int>("bidexchange", bid_exchange);
			parser.get<uint64_t>("timestamp", timestamp);
		}
	};

	struct LastQuote {
		std::string status;
		std::string symbol;
		Quote quote;

	private:
		template<typename> friend class Response;

		template<typename T>
		void fromJSON(const T& parser) {
			parser.get<std::string>("status", status);
			parser.get<std::string>("symbol", symbol);

			if (parser.json.HasMember("last") && parser.json["last"].IsObject()) {
				auto obj = parser.json["last"].GetObject();
				Parser<decltype(parser.json["last"].GetObject())> p(obj);
				quote.fromJSON(p);
			}
		}
	};
} // namespace alpaca