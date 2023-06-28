#include "OrderBook.h"

struct CompareSellOrder {
    bool operator()(Order& OrderA, Order& OrderB) {
        if (OrderA.stock.getPrice() != OrderB.stock.getPrice()) {
            return OrderA.stock.getPrice() < OrderB.stock.getPrice();
        }
        else {
            return OrderA.timestamp > OrderB.timestamp;
        }
    }
};

struct CompareOrder {
    bool operator()(Order& OrderA, Order& OrderB) {
        if (OrderA.stock.getPrice() != OrderB.stock.getPrice()) {
            return OrderA.stock.getPrice() > OrderB.stock.getPrice();
        }
        else {
            return OrderA.timestamp > OrderB.timestamp;
        }
    }
};

class OrderBook {
public:
    std::priority_queue<Order, std::vector<Order>, CompareOrder> buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrder> sellOrders;

    void addOrder(Order order) {
        if(order.type == OrderType::Buy) {
            buyOrders.push(order);
        } 
        else{
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

    std::vector<Order> executeTrades() {
        while(!buyOrders.empty() && !sellOrders.empty()) {
            Order buyOrder = buyOrders.top();
            Order sellOrder = sellOrders.top();

            if(buyOrder.stock.name == sellOrder.stock.name && buyOrder.stock.getPrice() >= sellOrder.stock.getPrice()) {
                Trade trade(buyOrder, sellOrder);

                buyOrder.quantity -= trade.tradeQuantity;
                sellOrder.quantity -= trade.tradeQuantity;

                if(buyOrder.quantity == 0) {
                    buyOrders.pop();
                }
                if(sellOrder.quantity == 0) {
                    sellOrders.pop();
                }
                std::vector<Order> orders;
                orders.push_back(buyOrder);
                orders.push_back(sellOrder);
                return orders;
            } 
            else {
                break;
            }
        }
    }

};
