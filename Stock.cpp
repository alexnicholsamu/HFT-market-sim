#include "Stock.h"

class Stock {
public:
    std::string name;
    double price;
    MarketEvent* marketEvent;

    Stock(std::string name, double price, MarketEvent* marketEvent): name(name), price(price), marketEvent(marketEvent) {}

    double getPrice(){
        return marketEvent->getFactor()*price;
    }
};