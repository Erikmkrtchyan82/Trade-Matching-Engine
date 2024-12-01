#ifndef TRADER_HPP
#define TRADER_HPP

#include <vector>
#include <iostream>

#include "../headers/order.hpp"

using TradeType = std::vector<Order>;

class Trader {
    public:
        Trader() = default;
        void add_trade(TradeType&& trade);
        friend std::ostream &operator<<(std::ostream &os, const Trader& trade);
    private:
        std::vector<TradeType> _trades;
};

#endif
