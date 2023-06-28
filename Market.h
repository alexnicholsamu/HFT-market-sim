#include <queue>
#include <vector>
#include <iostream>
#include <map>

#include "Trader.h"
#include "OrderBook.h"

class Market{
public:
    std::vector<Trader> traders;
    OrderBook* orderbook;
    Market(): traders(traders), orderbook(orderbook) {}
    // ill do orderbook stuff here
    // basically overarching class
};