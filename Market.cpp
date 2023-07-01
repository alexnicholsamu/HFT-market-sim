#include "Market.h"

class Market{
public:
    std::vector<Trader> traders;
    std::shared_ptr<OrderBook> orderbook;
    double interestRate = 1.0;
    double factors = 1.0;
    std::mutex mtx;
    MarketEvent ME;
    std::map<std::string, std::shared_ptr<Stock>> stocks;

    Market(): traders(traders), orderbook(orderbook), interestRate(interestRate), factors(factors), ME(ME), stocks(stocks) {}

    void executeOrderBook(){
        std::vector<std::shared_ptr<Order>> orders = orderbook->executeTrades();
        std::shared_ptr<Order> buyOrder = orders[0];
        std::shared_ptr<Order> sellOrder = orders[1];
        for(Trader& trader : traders){
            if(trader.id == buyOrder->id){
                trader.updatePortfolio(buyOrder);
            }
            if(trader.id == sellOrder->id){
                trader.updatePortfolio(sellOrder);
            }
        }
    }

    void generateMarketEvent(MarketEventType type, double impact){
        ME = MarketEvent(type, impact);
        applyMarketImpact(ME);
        std::chrono::seconds sleepDuration(40);
        std::this_thread::sleep_for(sleepDuration);
    }

    void fluctuateMarket(){
        std::vector<double> fluctuations = ME.generateRandomChange();
        for (auto& pair : stocks) { // directly apply fluctuations to stocks
            pair.second->fluctuate(fluctuations);
        }
    }

    void applyMarketImpact(MarketEvent& ME){
        std::lock_guard<std::mutex> lock(mtx);
        switch(ME.type){
            case MarketEventType::InterestRateChange:
                factors = ME.applyInterestImpact(interestRate, factors);
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->updateFactors(factors);
                }
            case MarketEventType::GlobalEconomy:
            case MarketEventType::EconomicIndicatorReports:
            case MarketEventType::PublicOpinion:
            case MarketEventType::Recession:
            case MarketEventType::Prosperity:
            case MarketEventType::OtherGovPolicy:
        }
    }

    void addStock(std::string name, double initialPrice){
        std::shared_ptr<Stock> newStock = std::make_shared<Stock>(name, initialPrice);
        stocks[name] = newStock;
        std::chrono::seconds sleepDuration(20);
        std::this_thread::sleep_for(sleepDuration);
    }

    void run(){
        bool running = true;
        while(running){

        }
    }

    void addTrader(){
        double id;
        double available_cash;
        traders.push_back(Trader(id, available_cash, orderbook));
        std::chrono::seconds sleepDuration(60);
        std::this_thread::sleep_for(sleepDuration);
    }

    void reset(){
        traders.clear();
        stocks.clear();
        orderbook->clear();
    }
};