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
    std::vector<std::shared_ptr<Stock>> stocks;
    std::shared_ptr<OrderBook> orderbook;
    Market();
    void executeOrderBook(std::mutex& mtx);
    void generateMarketEvent(std::map<double, MarketEventType> MEcreation, std::mutex& mtx);
    void applyMarketImpact(MarketEventType ME, std::mutex& mtx);
    void initializeStocks(std::string filename);
    void initializeTraders(std::string filename);
    std::map<double,MarketEventType> generateMarketEventChances();
    void reset();
    void fluctuateMarket(std::mutex& mtx);
    void run();
};

#endif