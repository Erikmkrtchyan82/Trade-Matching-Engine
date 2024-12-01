#include "../headers/order.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>

namespace {
void validate_trade_identifier(const std::string& trade_identifier) {
    if (trade_identifier.empty()) {
        throw std::invalid_argument("Trader identifier must not be empty.");
    }

    if (!std::all_of(trade_identifier.begin(), trade_identifier.end(), [](char c) {return std::isalnum(c);})) {
        throw std::invalid_argument("Trader identifier must be alphanumeric.");
    }
}

void validate_side(const std::string& side) {
    if (side.empty()) {
        throw std::invalid_argument("Side must not be empty.");
    }

    if (side != "B" && side != "S") {
        throw std::invalid_argument("Side must be either 'B' or 'S'.");
    }
}

void validate_quantity(const std::string& quantity) {
    if (quantity.empty()) {
        throw std::invalid_argument("Quantity must not be empty.");
    }

    if (!std::all_of(quantity.begin(), quantity.end(), [](char c) {return std::isdigit(c);})) {
        throw std::invalid_argument("Quantity must be a positive number.");
    }
}

void validate_price(const std::string& price) {
    if (price.empty()) {
        throw std::invalid_argument("Price must not be empty.");
    }

    if (!std::all_of(price.begin(), price.end(), [](char c) {return std::isdigit(c);})) {
        throw std::invalid_argument("Price must be a positive number.");
    }
}
}

Order::Order(std::string order) {
    std::istringstream iss(order);
    std::string trader_identifier;
    std::string side;
    std::string quantity;
    std::string price;

    iss >> trader_identifier >> side >> quantity >> price;
    iss.clear();

    if (trader_identifier.empty() || side.empty() || quantity.empty() || price.empty()) {
        throw std::invalid_argument("Order must have four fields: <Trader Identifier> <Side> <Quantity> <Price>");
    }
    validate_trade_identifier(trader_identifier);
    this->_trader_identifier = trader_identifier;

    validate_side(side);
    this->_side = side == "B" ? Side::BUY : Side::SELL;

    validate_quantity(quantity);
    this->_quantity = std::stoi(quantity);
    if (this->_quantity <= 0) {
        throw std::invalid_argument("Quantity must be greater than 0.");
    }

    validate_price(price);
    this->_price = std::stoi(price);
    if (_price <= 0) {
        throw std::invalid_argument("Price must be greater than 0.");
    }
}

Order::Order(const Order& other) {
    this->_trader_identifier = other._trader_identifier;
    this->_side = other._side;
    this->_quantity = other._quantity;
    this->_price = other._price;
}

unsigned Order::price() const {
    return this->_price;
}

void Order::set_price(unsigned price) {
    this->_price = price;
}


unsigned Order::quantity() const {
    return this->_quantity;
}

void Order::set_quantity(unsigned quantity) {
    static_assert(std::is_unsigned<decltype(quantity)>::value, "Quantity must be an unsigned integer.");
    this->_quantity = quantity;
}

Side Order::side() const {
    return this->_side;
}

std::string Order::trader_identifier() const {
    return this->_trader_identifier;
}

std::ostream& operator<<(std::ostream& os, const Side& side) {
    os << (side == Side::BUY ? 'B' : 'S');
    return os;
}

std::ostream &operator<<(std::ostream &os, const Order& order) {
    os << order.trader_identifier() << " " << order.side() << " " << order.quantity() << " " << order.price();
    return os;
}
