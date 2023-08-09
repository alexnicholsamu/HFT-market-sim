#include "Portfolio.h"

Portfolio::Portfolio()  {}

double Portfolio::makeChange(std::shared_ptr<Order> order, double cash){
    /*
        The results of a OrderBook execution is applied to the portfolio
    */
    if(order->type == OrderType::Buy){
        holdings[order->stock] += order->quantity;
    }
    else{
        holdings[order->stock] -= order->quantity;
        cash += order->order_price * order->quantity;
        if(holdings[order->stock] == 0){
            holdings.erase(order->stock);
        }
    }
    return cash;
}

void Portfolio::cancelSell(std::shared_ptr<Order> order){
    /*
        Self-explanatory, I hope
    */
    holdings[order->stock] += order->quantity;
}

std::vector<std::shared_ptr<Stock>> Portfolio::listStocks(){
    /*
        Helper method when deciding a sell
    */
    std::vector<std::shared_ptr<Stock>> listComps;
    for(const auto& pair : holdings) {
        std::shared_ptr<Stock> stock = pair.first;  
        listComps.push_back(stock);  
    }
    return listComps;
}

void Portfolio::boughtStock(std::shared_ptr<Stock> stock, int quantity){
    /*
        Raw buy, not bought on the market
    */
    holdings[stock] += quantity;
    std::cout << "Stock Bought!" << std::endl;
}

double Portfolio::soldStock(std::shared_ptr<Stock> stock, int quantity, double cash){
    /*
        Raw sell, not sold on the market 
    */
    holdings[stock] -= quantity;
    cash += stock->getPrice();
    if(holdings[stock] == 0){
        holdings.erase(stock);
    }
    std::cout << "Stock Sold!" << std::endl;
    return cash;
}