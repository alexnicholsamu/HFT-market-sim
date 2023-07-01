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
    std::shared_ptr<OrderBook> orderbook;
    Market(): traders(traders), orderbook(orderbook) {}
    void executeOrderBook();
    void generateMarketEvent(MarketEventType type, double impact);
    void applyMarketImpact(MarketEvent ME);
    void addStock(std::string name, double initialPrice);
    void addTrader();
    void reset();
    void run();
};