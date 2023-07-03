#ifndef ORDERBOOK_H
#define ORDERBOOK_H


#include "Order.h"
#include "Trade.h"

struct CompareSellOrder {
    bool operator()(std::shared_ptr<Order> OrderA, std::shared_ptr<Order> OrderB) {
        if (OrderA->stock->getPrice() != OrderB->stock->getPrice()) {
            return OrderA->stock->getPrice() < OrderB->stock->getPrice();
        }
        else {
            return OrderA->timestamp > OrderB->timestamp;
        }
    }
};

struct CompareOrder {
    bool operator()(std::shared_ptr<Order> OrderA, std::shared_ptr<Order> OrderB) {
        if (OrderA->stock->getPrice() != OrderB->stock->getPrice()) {
            return OrderA->stock->getPrice() > OrderB->stock->getPrice();
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
    std::mutex mtx;
    OrderBook();
    void addOrder(std::shared_ptr<Order> order);
    std::vector<std::shared_ptr<Order>> executeTrades();
    bool cancelOrder(std::shared_ptr<Order> order);
    void clear();
};

#endif