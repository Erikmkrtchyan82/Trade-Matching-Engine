#include "../headers/market.hpp"
#include "../headers/order.hpp"
#include <memory>

#include <iostream>

namespace {
Order _create_order_for_trade(const Order& order, unsigned quantity, unsigned price) {
    Order trade_order(order);
    trade_order.set_quantity(quantity);
    trade_order.set_price(price);
    return trade_order;
}
}

void Market::add_order(const std::string& order_str) {
    auto aggressor = std::make_shared<Order>(order_str);

    if(this->_process_order(aggressor)){
        this->_resting_orders.push_back(std::move(aggressor));
    }
}

bool Market::_process_order(std::shared_ptr<Order>& aggressor) {
    TradeType trade;
    std::vector< std::shared_ptr<Order> > to_remove;

    unsigned(*get_price)(unsigned, unsigned) = nullptr;
    auto sorted_orders = this->_resting_orders;

    // If the aggressor is a buy order, sort the resting orders by price in ascending order.
    if (aggressor->side() == Side::BUY) {
        sorted_orders.sort([](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b) {
            return a->price() < b->price();
        });
        get_price = [](unsigned a, unsigned b) { return std::min(a, b); };
    // If the aggressor is a sell order, sort the resting orders by price in descending order.
    } else {
        sorted_orders.sort([](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b) {
            return a->price() > b->price();
        });
        get_price = [](unsigned a, unsigned b) { return std::max(a, b); };
    }

    for (auto& order : sorted_orders) {
        if (aggressor->quantity() == 0) break;
        if (order->trader_identifier() == aggressor->trader_identifier()) continue;
        if (order->side() == aggressor->side()) continue;

        // If the aggressor is a buy order then the aggressor's price must be greater than or equal to the resting order's price.
        if (aggressor->side() == Side::BUY && order->price() > aggressor->price()) break;
        // If the aggressor is a sell order then the aggressor's price must be less than or equal to the resting order's price.
        if (aggressor->side() == Side::SELL && order->price() < aggressor->price()) break;

        auto trade_quantity = std::min(order->quantity(), aggressor->quantity());

        auto best_price = get_price(order->price(), aggressor->price());

        order->set_quantity(order->quantity() - trade_quantity);
        aggressor->set_quantity(aggressor->quantity() - trade_quantity);

        trade.push_back(_create_order_for_trade(*order, trade_quantity, best_price));
        trade.push_back(_create_order_for_trade(*aggressor, trade_quantity, best_price));

        if (order->quantity() == 0) {
            to_remove.push_back(order);
        }
    }

    this->_remove_orders(to_remove);
    this->_trader.add_trade(std::move(trade));

    return aggressor->quantity() > 0;
}

void Market::_remove_orders(const std::vector< std::shared_ptr<Order> >& to_remove) {
    for (auto& order : to_remove) {
        this->_resting_orders.remove(order);
    }
}

std::ostream &operator<<(std::ostream &os, const Market& market) {
    for (auto& order : market._resting_orders) {
        os << *order << std::endl;
    }
    return os;
}
