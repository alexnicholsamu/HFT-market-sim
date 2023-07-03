#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Order.h"

class Portfolio {
public:
    std::map<std::shared_ptr<Stock>, int> holdings; 
    std::mutex mtx;
    Portfolio();
    double portfolioValue();
    double makeChange(std::shared_ptr<Order> order, double cash);
    std::vector<std::shared_ptr<Stock>> listStocks();
    void cancelSell(std::shared_ptr<Order> order);
};

#endif