#include <queue>
#include <vector>
#include <iostream>
#include <map>

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

    Trader(int id, double available_cash, std::shared_ptr<OrderBook> orderbook);

    void makeOrder(OrderType type, std::shared_ptr<Stock> stock, int quantity, int id);
    void updatePortfolio(std::shared_ptr<Order> order);
    void cancelOrder(std::shared_ptr<Order> order);
    void getPositions();
    void getPortfolioValue();
    void getTotalValue();
};