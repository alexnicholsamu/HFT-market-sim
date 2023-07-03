#ifndef TRADE_H
#define TRADE_H

#include <queue>
#include <vector>
#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#include "Order.h"

class Trade{
public:
    const std::shared_ptr<Order> buyOrder;
    const std::shared_ptr<Order> sellOrder;
    std::chrono::system_clock::time_point timestamp;
    const int tradeQuantity;

    Trade(std::shared_ptr<Order> buyOrder, std::shared_ptr<Order> sellOrder);
};

#endif