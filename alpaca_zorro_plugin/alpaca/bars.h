#pragma once

#include <map>
#include <string>
#include <vector>

#include "rapidjson/document.h"

namespace alpaca {

	struct Bar {
		uint32_t time;
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		uint32_t volume;

	private:
		template<typename> friend class Response;
		friend class Bars;
		void fromJSON(rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>&d);
	};

	/**
	 * @brief A type representing bars for multiple symbols
	 */
	class Bars {
	public:
		std::map<std::string, std::vector<Bar>> bars;

	private:
		template<typename> friend class Response;
		void fromJSON(const rapidjson::Document& d);
	};
} // namespace alpaca
