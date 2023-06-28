#include <queue>
#include <vector>
#include <iostream>
#include <map>
#include <chrono>

#include "Order.h"

class Trade{
public:
    Order buyOrder;
    Order sellOrder;
    std::chrono::system_clock::time_point timestamp;
    int tradeQuantity;

    Trade(Order buyOrder, Order sellOrder);
};