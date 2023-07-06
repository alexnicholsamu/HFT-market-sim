#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "Trade.h"

struct CompareSellOrder {
    bool operator()(std::shared_ptr<Order> OrderA, std::shared_ptr<Order> OrderB) {
        if (OrderA->order_price != OrderB->order_price) {
            return OrderA->order_price < OrderB->order_price;
        }
        else {
            return OrderA->timestamp > OrderB->timestamp;
        }
    }
};

struct CompareOrder {
    bool operator()(std::shared_ptr<Order> OrderA, std::shared_ptr<Order> OrderB) {
        if (OrderA->order_price != OrderB->order_price) {
            return OrderA->order_price > OrderB->order_price;
        }
        else {
            return OrderA->timestamp > OrderB->timestamp;
        }
    }
};

class OrderBook {
public:
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareOrder> buyOrders;
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareSellOrder> sellOrders;
    OrderBook();
    void addOrder(std::shared_ptr<Order> order, std::mutex& trademtx);
    std::vector<std::shared_ptr<Order>> executeTrades(std::mutex& ordmtx);
    bool cancelOrder(std::shared_ptr<Order> order, std::mutex& trademtx);
    void clear();
};

#endif