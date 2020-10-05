#pragma once

#include <string>
#include <cassert>
#include "rapidjson/document.h"

namespace alpaca {

	/**
	 * @brief The class of an asset.
	 */
	enum AssetClass {
		USEquity,
	};

	/**
	 * @brief A helper to convert an AssetClass to a string
	 */
	inline constexpr const char* assetClassToString(AssetClass asset_class) {
		constexpr const char* sAssetClassToString[] = { "USEquity" };
		assert(asset_class == AssetClass::USEquity);
		return sAssetClassToString[asset_class];
	}

	/**
	 * @brief A type representing an Alpaca asset.
	 */
	struct Asset {
		bool easy_to_borrow;
		bool marginable;
		bool shortable;
		bool tradable;
		std::string asset_class;
		std::string exchange;
		std::string id;
		std::string status;
		std::string symbol;

	private:
		template<typename> friend class Response;
		void fromJSON(const rapidjson::Document& d);
	};
} // namespace alpaca