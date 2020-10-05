#include "stdafx.h"
#include "alpaca/asset.h"
#include "alpaca/json.h"

namespace alpaca {

	void Asset::fromJSON(const rapidjson::Document& d) {
		asset_class = parse<std::string>(d, "class");
		easy_to_borrow = parse<bool>(d, "easy_to_borrow");
		exchange = parse<std::string>(d, "exchange");
		id = parse<std::string>(d, "id");
		marginable = parse<bool>(d, "marginable");
		shortable = parse<bool>(d, "shortable");
		status = parse<std::string>(d, "status");
		symbol = parse<std::string>(d, "symbol");
		tradable = parse<bool>(d, "tradable");
	}
} // namespace alpaca