#include "OrderBook.h"


OrderBook::OrderBook() {} 

void OrderBook::addOrder(std::shared_ptr<Order> order, std::mutex& trademtx) {
    std::lock_guard<std::mutex> guard(trademtx);
    switch (order->type){
        case OrderType::Buy:
            buyOrders.push(order);
            break;
        case OrderType::Sell:
            sellOrders.push(order);
            break;
    }
    std::cout << "Order Placed!" << std::endl;
}

std::vector<std::shared_ptr<Order>> OrderBook::executeTrades(std::mutex& ordmtx) {
    std::lock_guard<std::mutex> guard(ordmtx);
    std::vector<std::shared_ptr<Order>> orders;
    while(!buyOrders.empty() && !sellOrders.empty()) {
        std::shared_ptr<Order> buyOrder = buyOrders.top();
        std::shared_ptr<Order> sellOrder = sellOrders.top();
        std::cout << "exec Order Book checkpoint 2" << std::endl;
        // Check if the orders are for the same stock
        if(buyOrder->stock->name != sellOrder->stock->name) {
            break;
        }
        
        // Proceed only if the prices match or if one of the orders is a market order
        if(buyOrder->order_price >= sellOrder->order_price || buyOrder->pref == OrderPreference::Market 
            || sellOrder->pref == OrderPreference::Market) {
            Trade trade(buyOrder, sellOrder);
            std::cout << "exec Order Book checkpoint 3" << std::endl;
            buyOrder->quantity -= trade.tradeQuantity;
            sellOrder->quantity -= trade.tradeQuantity;

            if(buyOrder->quantity == 0) {
                buyOrders.pop();
                buyOrder->status = OrderStatus::Closed;
            }
            else {
                buyOrder->status = OrderStatus::Partial;
            }
            if(sellOrder->quantity == 0) {
                sellOrders.pop();
                sellOrder->status = OrderStatus::Closed;
            }
            else {
                sellOrder->status = OrderStatus::Partial;
            }
            
            orders.push_back(buyOrder);
            orders.push_back(sellOrder);
            return orders;
        } 
        else {
            break;
        }
    }
    std::cout << "OrderBook executed!" << std::endl;
    return orders;
}

bool OrderBook::cancelOrder(std::shared_ptr<Order> order, std::mutex& trademtx){
    std::lock_guard<std::mutex> guard(trademtx);
    bool hadEntry = false;
    if(order->type == OrderType::Buy){
        std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareOrder> tempQueue;
        std::cout << "Trader action checkpoint 3.3" << std::endl;
        while(!buyOrders.empty()){
            std::shared_ptr<Order> currentEntry = buyOrders.top();
            buyOrders.pop();
            std::cout << "Trader action checkpoint 4.3" << std::endl;
            if(currentEntry->id == order->id && currentEntry->stock == order->stock 
                && currentEntry->quantity == order->quantity && !hadEntry){
                currentEntry->status = OrderStatus::Cancelled;
                hadEntry = true;
            } else {
                tempQueue.push(currentEntry);
            }
        }
        buyOrders = tempQueue;
        return hadEntry;
    }
    else{
        std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareSellOrder> tempQueue;
        while(!sellOrders.empty()){
            std::shared_ptr<Order> currentEntry = sellOrders.top();
            sellOrders.pop();
            std::cout << "Trader action checkpoint 6.3" << std::endl;
            if(currentEntry->id == order->id && currentEntry->stock == order->stock 
                && currentEntry->quantity == order->quantity && !hadEntry){
                currentEntry->status = OrderStatus::Cancelled;
                hadEntry = true;
            }
            else{
                tempQueue.push(currentEntry);
            }
        }
        sellOrders = tempQueue;
        return hadEntry;
    }
}

void OrderBook::clear(){
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareOrder>().swap(buyOrders);
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareSellOrder>().swap(sellOrders);
}
