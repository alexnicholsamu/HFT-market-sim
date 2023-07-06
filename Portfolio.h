#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Order.h"

class Portfolio {
public:
    std::map<std::shared_ptr<Stock>, int> holdings; 
    Portfolio();
    double makeChange(std::shared_ptr<Order> order, double cash, std::mutex& ordmtx);
    std::vector<std::shared_ptr<Stock>> listStocks(std::mutex& trademtx);
    void cancelSell(std::shared_ptr<Order> order, std::mutex& trademtx);
};

#endif