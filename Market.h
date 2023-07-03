#ifndef MARKET_H
#define MARKET_H

#include <queue>
#include <vector>
#include <iostream>
#include <map>
#include <variant>
#include <chrono>
#include <thread>
#include <fstream>
#include <atomic>

#include "Trader.h"
#include "OrderBook.h"

enum class MarketEventType { InterestRateChange, GlobalEconomy, EconomicIndicatorReports, 
    PublicOpinion, Recession, Prosperity, OtherGovPolicy, Nothing };

class Market{
public:
    std::vector<std::shared_ptr<Trader>> traders;
    std::mutex mtx;
    double interestRate = 1.0;
    double factors = 1.0;
    std::shared_ptr<OrderBook> orderbook;
    std::random_device rd;
    std::mt19937 generator;
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