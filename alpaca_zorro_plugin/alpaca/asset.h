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
		std::string asset_class;
		std::string exchange;
		std::string id;
		std::string status;
		std::string symbol;

	private:
		template<typename> friend class Response;

		template<typename CallerT, typename T>
		void fromJSON(const T& parser) {
			parser.get<std::string>("class", asset_class);
			parser.get<bool>("easy_to_borrow", easy_to_borrow);
			parser.get<std::string>("exchange", exchange);
			parser.get<std::string>("id", id);
			parser.get<bool>("marginable", marginable);
			parser.get<bool>("shortable", shortable);
			parser.get<std::string>("status", status);
			parser.get<std::string>("symbol", symbol);
			parser.get<bool>("tradable", tradable);
		}
	};
} // namespace alpaca