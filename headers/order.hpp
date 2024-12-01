#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <ostream>

enum class Side {
    BUY,
    SELL
};

std::ostream& operator<<(std::ostream& os, const Side& side);
class Order {
    public:
        Order(std::string);
        Order(const Order&);

        std::string trader_identifier() const;
        Side side() const;
        unsigned quantity() const;
        unsigned price() const;

        void set_price(unsigned price);
        void set_quantity(unsigned quantity);

        friend std::ostream &operator<<(std::ostream &os, const Order& order);

    private:
        std::string _trader_identifier;
        Side _side;
        unsigned _quantity;
        unsigned _price;
};

#endif
