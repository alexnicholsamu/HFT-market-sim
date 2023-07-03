#ifndef TRADER_H
#define TRADER_H

#include "Portfolio.h"
#include "Trade.h"
#include "OrderBook.h"
#include "Order.h"

class Trader{
public:
    Portfolio portfolio;
    int id;
    double available_cash;
    std::shared_ptr<OrderBook> orderbook;
    std::random_device rd;
    std::mt19937 generator;
    std::vector<std::shared_ptr<Order>> active_orders;

    Trader(int id, double available_cash, std::shared_ptr<OrderBook> orderbook);

    void makeOrder(OrderType type, std::shared_ptr<Stock> stock, int quantity, OrderPreference pref);
    void updatePortfolio(std::shared_ptr<Order> order);
    void cancelOrder(std::shared_ptr<Order> order);
    void doAction(std::vector<std::shared_ptr<Stock>> stocks);
};

#endif