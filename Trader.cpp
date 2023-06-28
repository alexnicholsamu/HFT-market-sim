#include "Trader.h"

class Trader{
public:
    Portfolio portfolio;
    int id;
    double available_cash;
    OrderBook* orderbook;

    Trader(int id, double available_cash): portfolio(portfolio), id(id), 
        available_cash(available_cash), orderbook(orderbook) {}
    
    void makeOrder(Order order){
        orderbook->addOrder(order, id);
    }

    
};