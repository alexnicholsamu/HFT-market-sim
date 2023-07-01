#include "OrderBook.h"

class OrderBook {
public:
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareOrder> buyOrders;
    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareSellOrder> sellOrders;

    OrderBook() = default;

    void addBuyOrder(std::shared_ptr<Order> order) {
        buyOrders.push(order);
    }
    void addSellOrder(std::shared_ptr<Order> order) {
        sellOrders.push(order);
    }

    std::shared_ptr<Order> grabBuyOrder() {
        if(buyOrders.empty()) {
            throw std::out_of_range("No more buy orders");
        }
        std::shared_ptr<Order> nextOrder = buyOrders.top();
        buyOrders.pop();
        return nextOrder;
    }

    std::shared_ptr<Order> grabSellOrder() {
        if(sellOrders.empty()) {
            throw std::out_of_range("No more sell orders");
        }
        std::shared_ptr<Order> nextOrder = sellOrders.top();
        sellOrders.pop();
        return nextOrder;
    }

    std::vector<std::shared_ptr<Order>> executeTrades() {
        while(!buyOrders.empty() && !sellOrders.empty()) {
            std::shared_ptr<Order> buyOrder = buyOrders.top();
            std::shared_ptr<Order> sellOrder = sellOrders.top();

            if(buyOrder->stock == sellOrder->stock && buyOrder->order_price >= sellOrder->order_price) {
                Trade trade(*buyOrder, *sellOrder);

                buyOrder->quantity -= trade.tradeQuantity;
                sellOrder->quantity -= trade.tradeQuantity;

                if(buyOrder->quantity == 0) {
                    buyOrders.pop();
                    buyOrder->status = OrderStatus::Closed;
                }
                else{
                    buyOrder->status = OrderStatus::Partial;
                }
                if(sellOrder->quantity == 0) {
                    sellOrders.pop();
                    sellOrder->status = OrderStatus::Closed;
                }
                else{
                    sellOrder->status = OrderStatus::Partial;
                }
                std::vector<std::shared_ptr<Order>> orders;
                orders.push_back(buyOrder);
                orders.push_back(sellOrder);
                return orders;
            } 
            else {
                break;
            }
        }
    }

    bool cancelOrder(std::shared_ptr<Order> order){
        if(order->type == OrderType::Buy){
            std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareOrder> tempQueue;
            bool hadEntry = false;
            while(!buyOrders.empty()){
                std::shared_ptr<Order> currentEntry = buyOrders.top();
                buyOrders.pop();
                if(currentEntry->id == order->id && currentEntry->stock == order->stock 
                    && currentEntry->quantity == order->quantity){
                    currentEntry->status = OrderStatus::Cancelled;
                    hadEntry = true;
                } else {
                    tempQueue.push(currentEntry);
                }
            }
            if(!hadEntry){
                std::cout << "Error: Orderbook did not contain Order!";
            }
            buyOrders = tempQueue;
            return hadEntry;
        }
        else{
            std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, CompareSellOrder> tempQueue;
            bool hadEntry = false;
            while(!sellOrders.empty()){
                std::shared_ptr<Order> currentEntry = sellOrders.top();
                sellOrders.pop();
                if(currentEntry->id == order->id && currentEntry->stock == order->stock 
                    && currentEntry->quantity == order->quantity){
                    currentEntry->status = OrderStatus::Cancelled;
                    hadEntry = true;
                    break;
                }
                else{
                    tempQueue.push(currentEntry);
                }
            }
            if(!hadEntry){
                std::cout << "Error: Orderbook did not contain Order!";
            }
            sellOrders = tempQueue;
            return hadEntry;
        }
    }
};
