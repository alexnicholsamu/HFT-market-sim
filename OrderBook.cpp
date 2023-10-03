#include "OrderBook.h"

OrderBook::OrderBook() {}

void OrderBook::addOrder(std::shared_ptr<Order> order, std::mutex& trademtx) {
    /*
        Adding a placed order to the OrderBook
    */
    std::lock_guard<std::mutex> guard(trademtx);
    if(order->type == OrderType::Buy) {
        buyOrders[order->stock->name].push(order);
        std::cout << "Buy Order for Trader " << order->id << " Placed!" << std::endl;
    } 
    else {
        sellOrders[order->stock->name].push(order);
        std::cout << "Sell Order for Trader " << order->id << " Placed!" << std::endl;
    }
}

std::vector<std::shared_ptr<Order>> OrderBook::executeTrades(std::mutex& ordmtx) {
    /*
        Where the orders are actually executed. The code loops through the buy orders and finds a matching (if available) sell
        order, and depending on whether the price / orderpreference, executes the trade / sell. All orders are added to a vector 
        and returned to be applied to the individual trader's portfolios
    */
    std::lock_guard<std::mutex> guard(ordmtx);
    std::vector<std::shared_ptr<Order>> orders;

    for(auto& [stock, buyQueue] : buyOrders) {
        auto& sellQueue = sellOrders[stock];
        while(!buyQueue.empty() && !sellQueue.empty()) {
            std::shared_ptr<Order> buyOrder = buyQueue.top();
            std::shared_ptr<Order> sellOrder = sellQueue.top();
            if(buyOrder->order_price >= sellOrder->order_price || buyOrder->pref == OrderPreference::Market 
                || sellOrder->pref == OrderPreference::Market) {
                Trade trade(buyOrder, sellOrder);
                buyOrder->quantity -= trade.tradeQuantity;
                sellOrder->quantity -= trade.tradeQuantity;
                if(buyOrder->quantity == 0) {
                    buyQueue.pop();
                    buyOrder->status = OrderStatus::Closed;
                }
                else {
                    buyOrder->status = OrderStatus::Partial;
                }
                if(sellOrder->quantity == 0) {
                    sellQueue.pop();
                    sellOrder->status = OrderStatus::Closed;
                }
                else {
                    sellOrder->status = OrderStatus::Partial;
                }
                orders.push_back(buyOrder);
                orders.push_back(sellOrder);
            } 
            else {
                break;
            }
        }
    }
    return orders;
}

bool OrderBook::cancelOrder(std::shared_ptr<Order> order, std::mutex& trademtx){
    /*
        A cancel order is found, and while it is normally expected to be an active order, there is a chance that the order is executed
        simultaneously as the order is searched for and cancelled, so there is safeguards in place. The status of the cancellation is 
        returned.

        There is room for improvement in the effiency of this part of code, but that is a non-trivial change and should be approached
        carefully (and may require a complete restructuring)
    */
    std::lock_guard<std::mutex> guard(trademtx);
    bool hadEntry = false;
    if(order->type == OrderType::Buy){
        auto& buyQueue = buyOrders[order->stock->name];
        std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareOrder> tempQueue;
        while(!buyQueue.empty()){
            std::shared_ptr<Order> currentEntry = buyQueue.top();
            buyQueue.pop();
            if(currentEntry->id == order->id && currentEntry->stock == order->stock 
                && currentEntry->quantity == order->quantity && !hadEntry){
                currentEntry->status = OrderStatus::Cancelled;
                hadEntry = true;
            } else {
                tempQueue.push(currentEntry);
            }
        }
        buyQueue = tempQueue;
    }
    else{
        auto& sellQueue = sellOrders[order->stock->name];
        std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareSellOrder> tempQueue;
        while(!sellQueue.empty()){
            std::shared_ptr<Order> currentEntry = sellQueue.top();
            sellQueue.pop();
            if(currentEntry->id == order->id && currentEntry->stock == order->stock 
                && currentEntry->quantity == order->quantity && !hadEntry){
                currentEntry->status = OrderStatus::Cancelled;
                hadEntry = true;
            }
            else{
                tempQueue.push(currentEntry);
            }
        }
        sellQueue = tempQueue;
    }
    return hadEntry;
}

void OrderBook::clear(){
    buyOrders.clear();
    sellOrders.clear();
}