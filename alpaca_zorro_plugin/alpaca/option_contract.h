#pragma once

#include <string>
#include <unordered_map>
#include <cassert>
#include "asset.h"

namespace alpaca {

	/**
	 * @brief A type representing an Alpaca asset.
	 */
	struct OptionContract : public AssetBase {
		friend struct OptionContracts;

		enum Type : uint8_t
		{
			OPT_CALL,
			OPT_PUT,
		};
		enum Style : uint8_t
		{
			OPT_AMERICAN,
			OPT_EUROPEAN,
		};

		std::string str_expration_date;
		std::string root_symbol;
		std::string underlying_symbol;
		std::string underlying_asset_id;
		std::string open_interest_date;
		std::string close_price_date;
		double strike_price = 0.0;
		double size = 0.0;
		double multiplier = 0.0;
		double close_price = 0.0;
		int64_t expiration_date;
		int32_t open_interest = 0;
		Type type;
		Style style;

	private:
		template<typename> friend class Response;

		template<typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			asset_class = AssetClass::OPTIONS;

			parser.get("id", id);
			status = toStatus(parser.get<std::string>("status"));
			type = toType(parser.get<std::string>("type"));
			style = toStyle(parser.get<std::string>("style"));
			parser.get("symbol", symbol);
			parser.get("tradable", tradable);
			parser.get("name", name);
			parser.get("expiration_date", str_expration_date);
			parser.get("root_symbole", root_symbol);
			parser.get("underlying_symbol", underlying_symbol);
			parser.get("underlying_asset_id", underlying_asset_id);
			parser.get("strike_price", strike_price);
			parser.get("size", size);
			parser.get("multiplier", multiplier);
			parser.get("open_interest", open_interest);
			parser.get("open_interest_date", open_interest_date);
			parser.get("close_price", close_price);
			parser.get("close_price_date", close_price_date);

			return std::make_pair(0, "OK");
		}

		static Style toStyle(const std::string& str_style)
		{
			static std::unordered_map<std::string, Style> styles = {
				{"american", Style::OPT_AMERICAN},
				{"european", Style::OPT_EUROPEAN},
			};
			assert(styles.find(str_style) != styles.end());
			return styles[str_style];
		}

		static Type toType(const std::string& str_type)
		{
			static std::unordered_map<std::string, Type> types = {
				{"call", Type::OPT_CALL},
				{"put", Type::OPT_PUT},
			};
			assert(types.find(str_type) != types.end());
			return types[str_type];
		}
	};

	struct OptionContracts
	{
		std::vector<OptionContract> contracts;
		std::string next_page_token;

	private:
		template<typename> friend class Response;

		template<typename T>
		std::pair<int, std::string> fromJSON(const T& parser) {
			parser.get<std::string>("next_page_token", next_page_token);
			if (parser.json.HasMember("option_contracts")) {
				for (auto& contract : parser.json["option_contracts"].GetArray()) {
					auto contractJson = contract.GetObject();
					Parser<decltype(contractJson)> p(contractJson);
					OptionContract op;
					op.fromJSON(p);
					contracts.emplace_back(std::move(op));
				}
			}
			return std::make_pair(0, "OK");
		}
	};

}