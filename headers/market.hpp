#ifndef MARKET_HPP
#define MARKET_HPP

#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "trader.hpp"
#include "order.hpp"

class Market {
    public:
        Market() = default;

        void add_order(const std::string& order_str);

        Trader trades() const { return this->_trader; }

        friend std::ostream &operator<<(std::ostream &os, const Market& market);
    private:
        bool _process_order(std::shared_ptr<Order>& aggressor);
        void _remove_orders(const std::vector< std::shared_ptr<Order> >& to_remove);

        std::list< std::shared_ptr<Order> > _resting_orders;
        Trader _trader;
};

#endif
