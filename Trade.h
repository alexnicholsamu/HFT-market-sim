#ifndef TRADE_H
#define TRADE_H

#include "Order.h"

class Trade{
public:
    const std::shared_ptr<Order> buyOrder;
    const std::shared_ptr<Order> sellOrder;
    const int tradeQuantity;

    Trade(std::shared_ptr<Order> buyOrder, std::shared_ptr<Order> sellOrder);
};

#endif