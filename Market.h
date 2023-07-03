#ifndef MARKET_H
#define MARKET_H

#include "Trader.h"

enum class MarketEventType { InterestRateChange, GlobalEconomy, EconomicIndicatorReports, 
    PublicOpinion, Recession, Prosperity, OtherGovPolicy, Nothing };

class Market{
public:
    std::random_device rd;
    std::mt19937 generator;
    std::vector<std::shared_ptr<Trader>> traders;
    std::mutex mtx;
    double interestRate = 1.0;
    double factors = 1.0;
    std::shared_ptr<OrderBook> orderbook;
    Market();
    void executeOrderBook();
    void generateMarketEvent(std::map<double, MarketEventType> MEcreation);
    void applyMarketImpact(MarketEventType ME);
    void initializeStocks(std::string filename);
    void initializeTraders(std::string filename);
    std::map<double,MarketEventType> generateMarketEventChances();
    std::vector<std::shared_ptr<Stock>> stocks;
    void reset();
    void fluctuateMarket();
    void run();
};

#endif