#include "stdafx.h"
#include "alpaca/bars.h"

#include "alpaca/json.h"

namespace alpaca {

	void Bar::fromJSON(rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>&d) {
		time = parse<uint32_t>(d, "t");
		open_price = parse<double>(d, "o");
		high_price = parse<double>(d, "h");
		low_price = parse<double>(d, "l");
		close_price = parse<double>(d, "c");
		volume = parse<uint32_t>(d, "v");
	}

	void Bars::fromJSON(const rapidjson::Document& d) {
		for (auto symbol_bars = d.MemberBegin(); symbol_bars != d.MemberEnd(); symbol_bars++) {
			bars[symbol_bars->name.GetString()] = std::vector<Bar>{};
			for (auto& symbol_bar : symbol_bars->value.GetArray()) {
				Bar bar;
				//bar.fromJSON(symbol_bar.GetObject());
				bars[symbol_bars->name.GetString()].emplace_back(std::move(bar));
			}
		}
	}
} // namespace alpaca