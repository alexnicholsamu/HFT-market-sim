#include <queue>
#include <vector>
#include <iostream>
#include <map>
#include <variant>
#include <chrono>
#include <thread>

#include "Trader.h"
#include "OrderBook.h"

class Market{
public:
    std::vector<Trader> traders;
    std::mutex mtx;
    double interestRate = 1.0;
    double factors = 1.0;
    MarketEvent ME;
    std::shared_ptr<OrderBook> orderbook;
    std::random_device rd;
    std::mt19937 generator;
    Market(): traders(traders), orderbook(orderbook), ME(ME) {}
    void executeOrderBook();
    void generateMarketEvent(std::map<double, MarketEventType> MEcreation);
    void applyMarketImpact(MarketEvent ME);
    void addStock(std::string name, double initialPrice);
    void addTrader();
    void reset();
    void run();
};