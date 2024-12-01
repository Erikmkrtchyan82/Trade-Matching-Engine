#include "../headers/trader.hpp"
#include <map>
#include <tuple>

const std::map<Side, std::string> TradeSign {
    {Side::BUY, "+"},
    {Side::SELL, "-"}
};

void Trader::add_trade(TradeType&& trade) {
    if (trade.size() <= 0) return;

    std::sort(trade.begin(), trade.end(), [](const Order& a, const Order& b) {
        return a.trader_identifier() < b.trader_identifier();
    });

    TradeType sumup;
    for (auto trade_order: trade) {
        bool merged = false;
        for (auto& sumup_order: sumup) {
            // Check if the trade order is the same as the sumup order we can merge the quantity.
            if (trade_order.trader_identifier() == sumup_order.trader_identifier() && sumup_order.price() == trade_order.price() && sumup_order.side() == trade_order.side()) {
                sumup_order.set_quantity(sumup_order.quantity() + trade_order.quantity());
                merged = true;
                break;
            }
        }
        if (!merged) {
            sumup.push_back(trade_order);
        }
    }

    // Sort the sumup by trader_identifier, side and price.
    std::sort(sumup.begin(), sumup.end(), [](const Order& a, const Order& b) {
        return std::make_tuple(a.trader_identifier(), TradeSign.at(a.side()), a.price()) < std::make_tuple(b.trader_identifier(), TradeSign.at(b.side()), b.price());
    });
    this->_trades.push_back(std::move(sumup));
}

std::ostream &operator<<(std::ostream &os, const Trader& trade) {
    for (const auto& trade_type : trade._trades) {
        for (const auto& order : trade_type) {
            os << order.trader_identifier() << TradeSign.at(order.side()) << order.quantity() << "@" << order.price() << " ";
        }
        os << std::endl;
    }
    return os;
}

