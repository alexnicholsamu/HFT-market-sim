#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Order.h"

class Portfolio {
public:
    std::map<std::shared_ptr<Stock>, int> holdings; 
    Portfolio();
    double makeChange(std::shared_ptr<Order> order, double cash);
    std::vector<std::shared_ptr<Stock>> listStocks();
    void cancelSell(std::shared_ptr<Order> order);
    void boughtStock(std::shared_ptr<Stock> stock, int quantity);
    double soldStock(std::shared_ptr<Stock> stock, int quantity, double cash);
};

#endif