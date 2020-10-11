#pragma once

#include <string>
#include <cassert>
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
        constexpr const char* sOrderDirection[] = { "Ascending", "Descending" };
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
        constexpr const char* sOrderSide[] = { "Buy", "Sell" };
        assert(side >= OrderSide::Buy && side <= OrderSide::Sell);
        return sOrderSide[side];
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
    };

    /**
     * @brief A helper to convert an OrderType to a string
     */
    inline constexpr const char* to_string(OrderType type)
    {
        constexpr const char* sOrderType[] = { "Market", "Limit", "Stop", "StopLimit" };
        assert(type >= OrderType::Market && type <= OrderType::StopLimit);
        return sOrderType[type];
    }

    /**
     * @brief Alpaca supports several Time-In-Force designations
     *
     * For more information on the supported designations, see:
     * https://alpaca.markets/docs/trading-on-alpaca/orders/#time-in-force
     */
    enum OrderTimeInForce : uint8_t {
        Day,
        GoodUntilCanceled,
        OPG,
        CLS,
        ImmediateOrCancel,
        FillOrKill,
    };

    /**
     * @brief A helper to convert an OrderTimeInForce to a string
     */
    inline constexpr const char* to_string(OrderTimeInForce tif) {
        constexpr const char* sTIF[] = { "Day", "GTC", "OPG", "CLS", "IOC", "FAK" };
        assert(tif >= OrderTimeInForce::Day && tif <= OrderTimeInForce::FillOrKill);
        return sTIF[tif];
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
        OneCancelsOther,
        OneTriggersOther,
    };

    /**
     * @brief A helper to convert an OrderClass to a string
     */
    inline constexpr const char* to_string(OrderClass order_class) {
        constexpr const char* sOrderClass[] = { "Simple", "Bracket", "OCO", "OTO" };
        assert(order_class >= OrderClass::Simple && order_class <= OrderClass::OneTriggersOther);
        return sOrderClass[order_class];
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
        std::string asset_class;
        std::string asset_id;
        std::string canceled_at;
        std::string client_order_id;
        std::string created_at;
        std::string expired_at;
        bool extended_hours;
        std::string failed_at;
        std::string filled_at;
        std::string filled_avg_price;
        std::string filled_qty;
        std::string id;
        bool legs;
        std::string limit_price;
        std::string qty;
        std::string side;
        std::string status;
        std::string stop_price;
        std::string submitted_at;
        std::string symbol;
        std::string time_in_force;
        std::string type;
        std::string updated_at;

    private:
        template<typename> friend class Response;

        template<typename T>
        void fromJSON(const T& parser) {

        }
    };
} // namespace alpaca