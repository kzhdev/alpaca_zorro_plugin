#pragma once

namespace alpaca {

struct Trade {
  double price;
  int size;
  int exchange;
  int cond1;
  int cond2;
  int cond3;
  int cond4;
  uint64_t timestamp;

  
private:
  template <typename> friend class Response;
  friend struct LastTrade;

  template <typename T>
  std::pair<int, std::string> fromJSON(const T& parser) {
    parser.get<double>("price", price);
    parser.get<int>("size", size);
    parser.get<int>("exchange", exchange);
    parser.get<int>("cond1", cond1);
    parser.get<int>("cond2", cond2);
    parser.get<int>("cond3", cond3);
    parser.get<uint64_t>("timestamp", timestamp);
    return std::make_pair(0, "OK");
  }
};

struct LastTrade {
  std::string status;
  std::string symbol;
  Trade trade;

private:
  template <typename> friend class Response;

  template <typename CallerT, typename T>
  std::pair<int, std::string> fromJSON(const T& parser) {
    parser.get<std::string>("status", status);
    parser.get<std::string>("symbol", symbol);

    if (status != "success") {
      return std::make_pair(1, symbol + " " + status);
    }

    if (parser.json.HasMember("last") && parser.json["last"].IsObject()) {
      auto obj = parser.json["last"].GetObject();
      Parser<decltype(parser.json["last"].GetObject())> p(obj);
      trade.fromJSON(p);
    }

    return std::make_pair(0, "OK");
  }
};
}