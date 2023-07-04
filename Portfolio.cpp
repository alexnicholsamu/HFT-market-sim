#include "Portfolio.h"

Portfolio::Portfolio()  {}

double Portfolio::portfolioValue(){
    double totvalue = 0;
    for(const auto& pair : holdings) {
        int quantity = pair.second;
        std::shared_ptr<Stock> stock = pair.first;  
        totvalue += quantity * stock->getPrice();  
    }
    return totvalue;
}

double Portfolio::makeChange(std::shared_ptr<Order> order, double cash, std::mutex& mtx){
    std::lock_guard<std::mutex> lock(mtx);
    if(order->type == OrderType::Buy){
        holdings[order->stock] += order->quantity;
    }
    else{
        holdings[order->stock] -= order->quantity;
        cash += order-> order_price * order->quantity;
        if(holdings[order->stock] == 0){
            holdings.erase(order->stock);
        }
    }
    return cash;
}

void Portfolio::cancelSell(std::shared_ptr<Order> order, std::mutex& mtx){
    std::lock_guard<std::mutex> guard(mtx);
    holdings[order->stock] += order->quantity;
}

std::vector<std::shared_ptr<Stock>> Portfolio::listStocks(){
    std::vector<std::shared_ptr<Stock>> listComps;
    for(const auto& pair : holdings) {
        std::shared_ptr<Stock> stock = pair.first;  
        listComps.push_back(stock);  
    }
    return listComps;
}

