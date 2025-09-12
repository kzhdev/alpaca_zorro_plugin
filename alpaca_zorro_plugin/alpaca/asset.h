#pragma once

#include <string>
#include <cassert>
#include <unordered_map>
//#include <market_data/market_data.h>

namespace alpaca {

	/**
	 * @brief The class of an asset.
	 */
	enum AssetClass : uint8_t {
		US_EQUITY,
		CRYPTO,
		CRYPTO_PERP,
		OPTIONS,
		__count__,	// for internal use only
	};

	/**
	 * @brief Asset Status.
	 */
	enum Status : uint8_t {
		ACTICE,
		INACTIVE
	};

	/**
	 * @brief A helper to convert an AssetClass to a string
	 */
	inline constexpr const char* assetClassToString(AssetClass asset_class) noexcept {
		constexpr const char* sAssetClassToString[] = { "us_equity", "crypto", "crypto_perp", "option" };
		static_assert(sizeof(sAssetClassToString) / sizeof(const char*) == AssetClass::__count__, "sAssetClassToString inconsistent with AssetClass");
		return sAssetClassToString[(uint8_t)asset_class];
	}

	inline AssetClass toAssetClass(const std::string& asset_class) noexcept {
		static std::unordered_map<std::string, AssetClass> assetClasses = {
			{"us_equity" , AssetClass::US_EQUITY},
			{"crypto", AssetClass::CRYPTO},
			{"crypto_perp", AssetClass::CRYPTO},
			{"option", AssetClass::OPTIONS},
		};
		assert(assetClasses.find(asset_class) != assetClasses.end());
		return assetClasses[asset_class];
	}

	inline Status toStatus(const std::string& str_status) noexcept {
		return str_status == "active" ? Status::ACTICE : Status::INACTIVE;
	}

	struct AssetBase
	{
		uint32_t index;
		std::string id;
		std::string symbol;
		std::string name;
		bool tradable;
		AssetClass asset_class;
		Status status;
	};

	/**
	 * @brief A type representing an Alpaca asset.
	 */
	struct Asset : public AssetBase {
		bool easy_to_borrow;
		bool marginable;
		bool shortable;
		bool fractionable;
		bool has_options = false;
		std::string exchange;

		// Crypto Asset field
		double min_order_size;
		double min_trade_increment;
		double price_increment;

	private:
		template<typename> friend class Response;

		template<typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			std::string str_asset_class;
			parser.get("class", str_asset_class);
			asset_class = toAssetClass(str_asset_class);
			parser.get("easy_to_borrow", easy_to_borrow);
			parser.get("exchange", exchange);
			parser.get("id", id);
			parser.get("marginable", marginable);
			parser.get("shortable", shortable);
			status = toStatus(parser.get<std::string>("status"));
			parser.get("symbol", symbol);
			parser.get("tradable", tradable);
			parser.get("fractionable", fractionable);
			parser.get("name", name);

			parser.get("min_order_size", min_order_size);
			parser.get("min_trade_increment", min_trade_increment);
			parser.get("price_increment", price_increment);

			if (parser.json.HasMember("attributes")) {
				for (auto& attr : parser.json["attributes"].GetArray()) {
					if (attr.GetString() == "has_options")
					{
						has_options = true;
						break;
					}
				}
			}
			return std::make_pair(0, "OK");
		}
	};
} // namespace alpaca