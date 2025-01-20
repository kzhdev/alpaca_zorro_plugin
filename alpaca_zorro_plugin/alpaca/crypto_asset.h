#pragma once

#include <string>
#include <unordered_map>
#include <cassert>
#include "json.h"
#include "asset.h"

namespace alpaca {

	/**
	 * @brief A type representing an Alpaca crypto asset.
	 */
	struct CryptoAsset : public Asset {
	private:
		template<typename> friend class Response;

		template<typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {

			auto kvp = Asset::fromJSON(parer);
			if (kvp.first)
			{
				// error occured
				return kvp;
			}
			parser.get("min_order_size", min_order_size);
			parser.get("min_trade_increment", min_trade_increment);
			parser.get("price_increment", price_increment);
			return std::make_pair(0, "OK");
		}
	};

}