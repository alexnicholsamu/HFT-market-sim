#include <string>
#include <vector>
#include <map>
#include <queue>

#include "Order.h"

class OrderBook {
private:
    std::priority_queue<Order> buyOrders;
    std::priority_queue<Order> sellOrders;

public: 
    void addOrder(Order order);
    Order grabBuyOrder();
    Order grabSellOrder();
};