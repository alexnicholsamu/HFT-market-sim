#include "Market.h"

class Market{
public:
    std::vector<Trader> traders;
    OrderBook* orderbook;


    void executeTrades(){
        std::vector<Order> orders = orderbook->executeTrades();
        Order buyOrder = orders[0];
        Order sellOrder = orders[1];
        for(Trader trader : traders){
            if(trader.id == buyOrder.id){
                trader.updatePortfolio(buyOrder);
            }
            if(trader.id == sellOrder.id){
                trader.updatePortfolio(sellOrder);
            }
        }
    }
   
};