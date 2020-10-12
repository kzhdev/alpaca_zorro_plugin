#pragma once

#include <string>
#include "alpaca/asset.h"

namespace alpaca {

	enum PositionSide : uint8_t {
		Long,
		Short
	};

	constexpr const char* to_string(PositionSide side) {
		constexpr const char* positionSide[] = {"long", "short"};
		return positionSide[side];
	}

	inline PositionSide to_positionSide(const std::string& side) {
		static std::unordered_map<std::string, PositionSide> positionSides = {
			{"long", PositionSide::Long},
			{"short", PositionSide::Short},
		};
		assert(positionSides.find(side) != positionSides.end());
		return positionSides[side];
	}

	struct Position {
		double avg_entry_price;
		double change_today;
		double cost_basis;
		double current_price;
		double lastday_price;
		double market_value;
		double unrealized_intraday_pl;
		double unrealized_intraday_plpc;
		double unrealized_pl;
		double unrealized_plpc;
		uint32_t qty;
		AssetClass asset_class;
		PositionSide side;
		std::string asset_id;
		std::string exchange;
		std::string symbol;

	private:
		template<typename> friend class Response;

		template<typename T>
		void fromJSON(const T& parser) {
			parser.get<double>("avg_entry_price", avg_entry_price);
			parser.get<double>("change_today", change_today);
			parser.get<double>("cost_basis", cost_basis);
			parser.get<double>("current_price", current_price);
			parser.get<double>("lastday_price", lastday_price);
			parser.get<double>("market_value", market_value);
			parser.get<double>("unrealized_intraday_pl", unrealized_intraday_pl);
			parser.get<double>("unrealized_intraday_plpc", unrealized_intraday_plpc);
			parser.get<double>("unrealized_pl", unrealized_pl);
			parser.get<double>("unrealized_pl", unrealized_plpc);
			parser.get<uint32_t>("qty", qty);
			asset_class = to_assetClass(parser.get<std::string>("asset_class"));
			side = to_positionSide(parser.get<std::string>("side"));
			parser.get<std::string>("asset_id", asset_id);
			parser.get<std::string>("exchange", exchange);
			parser.get<std::string>("symbol", symbol);
		}
	};
} // namespace alpaca