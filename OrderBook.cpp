#include "OrderBook.h"

// using operator(), which changes the structure of the prioirty queue to follow this manner instead
struct CompareSellOrder {
    bool operator()(Order& OrderA, Order& OrderB) {
        return OrderA.stock.getPrice()*OrderA.quantity > OrderB.stock.getPrice()*OrderB.quantity;
    }
};

class OrderBook {
public:
    std::priority_queue<Order> buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrder> sellOrders;

    void addOrder(Order order) {
        if(order.type == OrderType::Buy) {
            buyOrders.push(order);
        } 
        else{
            sellOrders.push(order);
        }
    }

    Order grabBuyOrder(){
        Order nextOrder = buyOrders.top();
        buyOrders.pop();
        return nextOrder;
    }

    Order grabSellOrder(){
        Order nextOrder = sellOrders.top();
        sellOrders.pop();
        return nextOrder;
    }

};