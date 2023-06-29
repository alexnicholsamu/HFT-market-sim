#include "Market.h"

class Market{
public:
    std::vector<Trader> traders;
    OrderBook* orderbook;
    double interestRate = 1.0;
    double factors = 1.0;
    MarketEvent ME;
    std::map<std::string, Stock*> stocks;

    Market(): traders(traders), orderbook(orderbook), interestRate(interestRate), factors(factors), ME(ME), stocks(stocks) {}

    void executeOrderBook(){
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

    void generateMarketEvent(MarketEventType type, double impact){
        ME = MarketEvent(type, impact);
        applyMarketImpact(ME);
    }

    void applyMarketImpact(MarketEvent ME){
        switch(ME.type){
            case MarketEventType::InterestRateChange:
                factors = ME.applyInterestImpact(interestRate, factors);
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->updateFactors(factors);
                }
            case MarketEventType::Fluctuation:
                std::vector<double> fluctuations = ME.generateRandomChange();
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->fluctuate(fluctuations);
                }
        }
    }

    void addStock(std::string name, double initialPrice){
        Stock* newStock = new Stock(name, initialPrice);
        stocks[name] = newStock;
    }
  
};