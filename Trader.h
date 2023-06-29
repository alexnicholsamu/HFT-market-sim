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
    OrderBook* orderbook;

    Trader(int id, double available_cash, OrderBook* orderbook);

    void makeOrder(OrderType type, Stock* stock, int quantity, int id);
    void updatePortfolio(Order* order);
    void cancelOrder(Order* order);
    void getPositions();
    void getPortfolioValue();
    void getTotalValue();
};