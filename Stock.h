#include <string>
#include <vector>
#include <map>

#include "MarketEvent.h"

class Stock{
public:
    std::string name;
    double price;
    MarketEvent* marketEvent;

public:
    Stock(std::string name, double price);
    double getPrice();
};