#pragma once

#include <string>
#include <cassert>
#include <unordered_map>

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
	inline constexpr const char* assetClassToString(AssetClass asset_class) noexcept {
		constexpr const char* sAssetClassToString[] = { "us_equity" };
		assert(asset_class == AssetClass::USEquity);
		return sAssetClassToString[asset_class];
	}

	inline AssetClass to_assetClass(const std::string& asset_class) noexcept {
		static std::unordered_map<std::string, AssetClass> assetClasses = {
			{"us_equity" , AssetClass::USEquity},
		};
		assert(assetClasses.find(asset_class) != assetClasses.end());
		return assetClasses[asset_class];
	}

	/**
	 * @brief A type representing an Alpaca asset.
	 */
	struct Asset {
		bool easy_to_borrow;
		bool marginable;
		bool shortable;
		bool tradable;
		bool fractionable;
		std::string asset_class;
		std::string exchange;
		std::string id;
		std::string status;
		std::string symbol;
		std::string name;

	private:
		template<typename> friend class Response;

		template<typename CallerT, typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			parser.get("class", asset_class);
			parser.get("easy_to_borrow", easy_to_borrow);
			parser.get("exchange", exchange);
			parser.get("id", id);
			parser.get("marginable", marginable);
			parser.get("shortable", shortable);
			parser.get("status", status);
			parser.get("symbol", symbol);
			parser.get("tradable", tradable);
			parser.get("fractionable", fractionable);
			parser.get("name", name);
			return std::make_pair(0, "OK");
		}
	};
} // namespace alpaca