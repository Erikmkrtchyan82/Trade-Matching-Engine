#include <iostream>
#include <string>

#include "../headers/market.hpp"

int main(int argc, char* argv[]) {
    std::string line;

    Market market;

    std::cout << "Enter order in form of <Trader Identifier> <Side> <Quantity> <Price> (hit <enter> for exit): " << std::endl;
    while (std::getline(std::cin, line)){
        if (line == "") break;
        try {
            market.add_order(line);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::cout << market.trades() << std::endl;

    return 0;
}
