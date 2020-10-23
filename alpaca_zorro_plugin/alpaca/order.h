#pragma once

#include <string>
#include <cassert>
#include <unordered_map>
#include "rapidjson/document.h"

namespace alpaca {

    /**
     * @brief The direction to display orders in when enumerating them.
     */
    enum OrderDirection : uint8_t {
        Ascending,
        Descending,
    };

    /**
     * @brief A helper to convert an OrderDirection to a string
     */
    inline constexpr const char* to_string(OrderDirection direction) {
        constexpr const char* sOrderDirection[] = { "asc", "desc" };
        assert(direction >= OrderDirection::Ascending && direction <= OrderDirection::Descending);
        return sOrderDirection[direction];
    }

    /**
     * @brief When you submit an order, you may be buying or selling.
     */
    enum OrderSide : uint8_t {
        Buy,
        Sell,
    };

    /**
     * @brief A helper to convert an OrderSide to a string
     */
    inline constexpr const char* to_string(OrderSide side) {
        constexpr const char* sOrderSide[] = { "buy", "sell" };
        assert(side >= OrderSide::Buy && side <= OrderSide::Sell);
        return sOrderSide[side];
    }

    inline OrderSide to_orderSide(const std::string& side) {
        return side == "buy" ? OrderSide::Buy : OrderSide::Sell;
    }

    /**
     * @brief When you submit an order, you can choose a supported order type.
     *
     * For more information on supported order types, see:
     * https://alpaca.markets/docs/trading-on-alpaca/orders/#order-types
     */
    enum OrderType : uint8_t {
        Market,
        Limit,
        Stop,
        StopLimit,
        TrailingStop,
    };

    /**
     * @brief A helper to convert an OrderType to a string
     */
    inline constexpr const char* to_string(OrderType type)
    {
        constexpr const char* sOrderType[] = { "market", "limit", "stop", "stop_limit", "trailing_stop" };
        assert(type >= OrderType::Market && type <= OrderType::StopLimit);
        return sOrderType[type];
    }

    inline OrderType to_orderType(const std::string& type) {
        static std::unordered_map<std::string, OrderType> orderTypes = {
            {"market", OrderType::Market},
            {"limit", OrderType::Limit},
            {"stop", OrderType::Stop},
            {"stop_limit", OrderType::StopLimit},
            {"trailing_stop", OrderType::TrailingStop}
        };
        assert(orderTypes.find(type) != orderTypes.end());
        return orderTypes[type];
    }

    /**
     * @brief Alpaca supports several Time-In-Force designations
     *
     * For more information on the supported designations, see:
     * https://alpaca.markets/docs/trading-on-alpaca/orders/#time-in-force
     */
    enum TimeInForce : uint8_t {
        Day,
        GTC,
        OPG,
        CLS,
        IOC,
        FOK,
    };

    /**
     * @brief A helper to convert an OrderTimeInForce to a string
     */
    inline constexpr const char* to_string(TimeInForce tif) {
        constexpr const char* sTIF[] = { "day", "gtc", "opg", "cls", "ioc", "fok" };
        assert(tif >= TimeInForce::Day && tif <= TimeInForce::FOK);
        return sTIF[tif];
    }

    inline TimeInForce to_timeInForce(const std::string& tif) {
        static std::unordered_map<std::string, TimeInForce> tifs = {
            {"day", TimeInForce::Day},
            {"gtc", TimeInForce::GTC},
            {"opg", TimeInForce::OPG},
            {"cls", TimeInForce::CLS},
            {"ioc", TimeInForce::IOC},
            {"fok", TimeInForce::FOK},
        };
        assert(tifs.find(tif) != tifs.end());
        return tifs[tif];
    }

    /**
     * @brief The class of the order
     *
     * For details of non-simple order classes, please see:
     * https://alpaca.markets/docs/trading-on-alpaca/orders/#bracket-orders
     */
    enum OrderClass : uint8_t {
        Simple,
        Bracket,
        OCO,
        OTO,
    };

    /**
     * @brief A helper to convert an OrderClass to a string
     */
    inline constexpr const char* to_string(OrderClass order_class) {
        constexpr const char* sOrderClass[] = { "simple", "bracket", "oco", "oto" };
        assert(order_class >= OrderClass::Simple && order_class <= OrderClass::OTO);
        return sOrderClass[order_class];
    }

    inline OrderClass to_orderClass(const std::string& order_class) {
        static std::unordered_map<std::string, OrderClass> orderClasses = {
            {"simple", OrderClass::Simple},
            {"bracket", OrderClass::Bracket},
            {"oco", OrderClass::OCO},
            {"oto", OrderClass::OTO},
        };
        assert(orderClasses.find(order_class) != orderClasses.end());
        return orderClasses[order_class];
    }

    /**
     * @brief Additional parameters for take-profit leg of advanced orders
     */
    struct TakeProfitParams {
        /// Required for bracket orders
        std::string limitPrice;
    };

    /**
     * @brief Additional parameters for stop-loss leg of advanced orders
     */
    struct StopLossParams {
        /// Required for bracket orders
        std::string stopPrice;
        /// The stop-loss order becomes a stop-limit order if specified
        std::string limitPrice;
    };

    /**
     * @brief A type representing an Alpaca order.
     */
    struct Order {
        double filled_avg_price = 0.;
        double limit_price = 0.;
        double stop_price = 0.;
        uint32_t qty = 0;
        uint32_t filled_qty = 0;
        int32_t internal_id = 0;
        AssetClass asset_class = AssetClass::USEquity;
        OrderSide side;
        TimeInForce tif;
        OrderType type;
        bool extended_hours = false;
        std::vector<Order> legs;

        std::string symbol;
        std::string asset_id;
        std::string canceled_at;
        std::string client_order_id;
        std::string created_at;
        std::string expired_at;
        std::string failed_at;
        std::string filled_at;
        std::string id;
        std::string status;
        std::string submitted_at;
        std::string updated_at;

    private:
        template<typename> friend class Response;

        template<typename CallerT, typename T>
        void fromJSON(const T& parser) {
            parser.get<std::string>("id", id);
            parser.get<std::string>("client_order_id", client_order_id);
            parser.get<std::string>("created_at", created_at);
            parser.get<std::string>("updated_at", updated_at);
            parser.get<std::string>("submitted_at", submitted_at);
            parser.get<std::string>("filled_at", filled_at);
            parser.get<std::string>("expired_at", expired_at);
            parser.get<std::string>("canceled_at", canceled_at);
            parser.get<std::string>("failed_at", failed_at);
            parser.get<std::string>("asset_id", asset_id);
            parser.get<std::string>("symbol", symbol);
            asset_class = to_assetClass(parser.get<std::string>("asset_class"));
            parser.get<uint32_t>("qty", qty);
            parser.get<uint32_t>("filled_qty", filled_qty);
            type = to_orderType(parser.get<std::string>("type"));
            side = to_orderSide(parser.get<std::string>("side"));
            tif = to_timeInForce(parser.get<std::string>("time_in_force"));
            parser.get<double>("limit_price", limit_price);
            parser.get<double>("stop_price", stop_price);
            parser.get<double>("filled_avg_price", filled_avg_price);
            parser.get<std::string>("status", status);
            parser.get<bool>("extended_hours", extended_hours);

            if (client_order_id.substr(0, 6) != "ZORRO_") {
                return;
            }
            auto pos = client_order_id.rfind("_");
            assert(pos != std::string::npos);
            auto internalOrderId = client_order_id.substr(pos + 1);
            if (!internalOrderId.empty() && internalOrderId.find_first_not_of("0123456789") == std::string::npos) {
                internal_id = atoi(internalOrderId.c_str());
            }
        }
    };
} // namespace alpaca