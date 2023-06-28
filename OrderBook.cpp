#include "OrderBook.h"

class OrderBook {
public:
    std::priority_queue<Order, std::vector<Order>, CompareOrder> buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrder> sellOrders;

    void addBuyOrder(Order order) {
        buyOrders.push(order);
    }
    void addSellOrder(Order order) {
        sellOrders.push(order);
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
                    buyOrder.status = OrderStatus::Closed;
                }
                else{
                    buyOrder.status = OrderStatus::Partial;
                }
                if(sellOrder.quantity == 0) {
                    sellOrders.pop();
                    sellOrder.status = OrderStatus::Closed;
                }
                else{
                    sellOrder.status = OrderStatus::Partial;
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

    bool cancelOrder(Order order){
        if(order.type == OrderType::Buy){
            std::priority_queue<Order, std::vector<Order>, CompareOrder> tempQueue = buyOrders;
            std::vector<Order> delVect;
            bool hadEntry = false;
            while(!tempQueue.empty()){
                Order currentEntry = tempQueue.top();
                tempQueue.pop();
                delVect.push_back(currentEntry);
                if(currentEntry.id == order.id && currentEntry.stock.name == order.stock.name 
                    && currentEntry.quantity == order.quantity){
                    delVect.pop_back();
                    order.status = OrderStatus::Cancelled;
                    hadEntry = true;
                }
            }
            if(!hadEntry){
                std::cout << "Error: Orderbook did not contain Order!";
            }
            else{
                buyOrders = std::priority_queue<Order, std::vector<Order>, CompareOrder>(delVect.begin(), delVect.end());
            }
            return hadEntry;
        }
        else{
            std::priority_queue<Order, std::vector<Order>, CompareSellOrder> tempQueue = sellOrders;
            std::vector<Order> delVect;
            bool hadEntry = false;
            while(!tempQueue.empty()){
                Order currentEntry = tempQueue.top();
                tempQueue.pop();
                delVect.push_back(currentEntry);
                if(currentEntry.id == order.id && currentEntry.stock.name == order.stock.name 
                    && currentEntry.quantity == order.quantity){
                    delVect.pop_back();
                    order.status = OrderStatus::Cancelled;
                    hadEntry = true;
                }
            }
            if(!hadEntry){
                std::cout << "Error: Orderbook did not contain Order!";
            }
            else{
                sellOrders = std::priority_queue<Order, std::vector<Order>, CompareSellOrder>(delVect.begin(), delVect.end());
            }
            return hadEntry;
        }
    }
};