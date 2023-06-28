#include <string>
#include <vector>
#include <map>
#include <queue>

#include "Order.h"
#include "Trade.h"


class OrderBook {
private:
    std::priority_queue<Order> buyOrders;
    std::priority_queue<Order> sellOrders;

public: 
    void addOrder(Order order);
    Order grabBuyOrder();
    Order grabSellOrder();
    std::vector<Order> executeTrades();
};