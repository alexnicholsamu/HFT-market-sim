#include "Portfolio.h"

Portfolio::Portfolio()  {}

double Portfolio::makeChange(std::shared_ptr<Order> order, double cash){
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

void Portfolio::cancelSell(std::shared_ptr<Order> order){
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

void Portfolio::boughtStock(std::shared_ptr<Stock> stock, int quantity){
    holdings[stock] += quantity;
    std::cout << "Stock Bought!" << std::endl;
}

double Portfolio::soldStock(std::shared_ptr<Stock> stock, int quantity, double cash){
    holdings[stock] -= quantity;
    cash += stock->getPrice();
    if(holdings[stock] == 0){
        holdings.erase(stock);
    }
    std::cout << "Stock Sold!" << std::endl;
    return cash;
}