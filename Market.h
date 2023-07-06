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
    std::mutex flucmtx;
    std::mutex ordmtx;
    std::mutex trademtx;
    std::mutex meventmtx;
    double interestRate = 1.0;
    double factors = 1.0;
    std::vector<std::shared_ptr<Stock>> stocks;
    std::shared_ptr<OrderBook> orderbook;
    Market();
    void executeOrderBook(std::mutex& ordmtx);
    void generateMarketEvent(std::map<double, MarketEventType> MEcreation, std::mutex& meventmtx);
    void applyMarketImpact(MarketEventType ME, std::mutex& meventmtx);
    void initializeStocks(std::string filename);
    void initializeTraders(std::string filename);
    std::map<double,MarketEventType> generateMarketEventChances();
    void reset();
    void fluctuateMarket(std::mutex& flucmtx);
    void run();
};

#endif