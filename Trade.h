#include <queue>
#include <vector>
#include <iostream>
#include <map>
#include <chrono>

#include "Order.h"

class Trade{
public:
    const Order buyOrder;
    const Order sellOrder;
    std::chrono::system_clock::time_point timestamp;
    const int tradeQuantity;

    Trade(Order buyOrder, Order sellOrder);
};