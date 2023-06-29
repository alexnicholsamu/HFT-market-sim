#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>

#include "Order.h"
#include "Trade.h"

struct CompareSellOrder {
    bool operator()(Order* OrderA, Order* OrderB) {
        if (OrderA->stock->getPrice() != OrderB->stock->getPrice()) {
            return OrderA->stock->getPrice() < OrderB->stock->getPrice();
        }
        else {
            return OrderA->timestamp > OrderB->timestamp;
        }
    }
};

struct CompareOrder {
    bool operator()(Order* OrderA, Order* OrderB) {
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
    std::priority_queue<Order*, std::vector<Order*>, CompareOrder> buyOrders;
    std::priority_queue<Order*, std::vector<Order*>, CompareSellOrder> sellOrders;

    OrderBook() = default;

public: 
    void addBuyOrder(Order* order);
    void addSellOrder(Order* order);
    Order* grabBuyOrder();
    Order* grabSellOrder();
    std::vector<Order*> executeTrades();
    bool cancelOrder(Order* order);
};