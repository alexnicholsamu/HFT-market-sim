#include "Trader.h"

class Trader{
public:
    Portfolio portfolio;
    int id;
    double available_cash;
    OrderBook* orderbook;

    Trader(int id, double available_cash, OrderBook* orderbook): portfolio(portfolio), id(id), 
        available_cash(available_cash), orderbook(orderbook) {}
    
    void makeOrder(OrderType type, Stock stock, int quantity, int id){
        Order order = Order(type, stock, quantity, id);
        orderbook->addOrder(order);
    }

    void updatePortfolio(Order order){
        available_cash = portfolio.makeChange(order, available_cash);
    }

};