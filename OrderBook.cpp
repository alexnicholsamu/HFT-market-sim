#include "OrderBook.h"

struct CompareSellOrder {
    bool operator()(Order& OrderA, Order& OrderB) {
        return OrderA.stock.getPrice() > OrderB.stock.getPrice();
    }
};

class OrderBook {
public:
    std::priority_queue<Order> buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrder> sellOrders;

    void addOrder(Order order, int id) {
        if(order.type == OrderType::Buy) {
            order.status = OrderStatus::Open;
            order.id = id;
            buyOrders.push(order);
        } 
        else{
            order.status = OrderStatus::Open;
            order.id = id;
            sellOrders.push(order);
        }
    }

    Order grabBuyOrder() {
        if(buyOrders.empty()) {
            throw std::out_of_range("No more buy orders");
        }
        Order nextOrder = buyOrders.top();
        buyOrders.pop();
        return nextOrder;
    }

    Order grabSellOrder() {
        if(sellOrders.empty()) {
            throw std::out_of_range("No more sell orders");
        }
        Order nextOrder = sellOrders.top();
        sellOrders.pop();
        return nextOrder;
    }

    void executeTrades() {
        while(!buyOrders.empty() && !sellOrders.empty()) {
            Order buyOrder = grabBuyOrder();
            Order sellOrder = grabSellOrder();

            if(buyOrder.stock.name == sellOrder.stock.name){
                Trade trade(buyOrder, sellOrder);
                // trade logic that needs to reference portfolio
            }
            else{
                // execute buy/sell in portfolio
            }
        }
    }
};
