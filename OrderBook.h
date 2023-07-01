#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <memory>

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

    OrderBook() = default;

public: 
    void addBuyOrder(std::shared_ptr<Order> order);
    void addSellOrder(std::shared_ptr<Order> order);
    std::shared_ptr<Order> grabBuyOrder();
    std::shared_ptr<Order> grabSellOrder();
    std::vector<std::shared_ptr<Order>> executeTrades();
    bool cancelOrder(std::shared_ptr<Order> order);
};