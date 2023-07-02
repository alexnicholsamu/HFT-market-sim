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
    std::random_device rd;
    std::mt19937 generator;

    Market(): traders(traders), orderbook(orderbook), interestRate(interestRate), factors(factors), 
        ME(ME), stocks(stocks), rd(), generator(rd()) {}

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

    void generateMarketEvent(std::map<double, MarketEventType> MEcreation){
        std::lock_guard<std::mutex> lock(mtx);
        std::uniform_int_distribution<double> distribution(0.0, 1.0);
        double marketEvent = distribution(generator);
        for(auto& pair : MEcreation){
            if (marketEvent <= pair.first) {
                ME = MarketEvent(pair.second);
                break;
            }
        }
        applyMarketImpact(ME);
        std::chrono::seconds sleepDuration(40);
        std::this_thread::sleep_for(sleepDuration);
    }

    void fluctuateMarket(){
        std::vector<double> marketFluctuations = ME.generateRandomChange();
        for (auto& pair : stocks) { // directly apply fluctuations to stocks
            pair.second->fluctuate(marketFluctuations);
        }
        std::chrono::seconds sleepDuration(2);
        std::this_thread::sleep_for(sleepDuration);
    }

    void applyMarketImpact(MarketEvent& ME){
        double impact;
        std::vector<double> fluctuations;
        switch(ME.type){
            case MarketEventType::InterestRateChange:
                std::uniform_int_distribution<double> interestDistribution(-4.0, 4.0);
                impact = interestDistribution(generator);
                factors = ME.applyInterestImpact(interestRate, factors, impact);
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->updateFactors(factors);
                }
                break;
            case MarketEventType::GlobalEconomy:
                std::uniform_int_distribution<double> GEdistribution(0.0, 0.25);
                impact = GEdistribution(generator);
                factors = ME.applyGlobalEconomy(factors, impact);
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->updateFactors(factors);
                }
                break;
            case MarketEventType::EconomicIndicatorReports:
                std::uniform_int_distribution<double> EIRdistribution(0.25, 0.50);
                impact = EIRdistribution(generator);
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->econIndicators(factors, impact);
                }
                break;
            case MarketEventType::PublicOpinion:
                std::uniform_int_distribution<double> lowdistribution(0.00, 0.60);
                std::uniform_int_distribution<double> highdistribution(0.40, 1.0);
                std::uniform_int_distribution<double> distribution(0.00, 1.0);
                double highlow = distribution(generator);
                double high = highdistribution(generator);
                double low = lowdistribution(generator);
                if(highlow<0.5){
                    fluctuations.push_back(highlow);
                    fluctuations.push_back(low);
                    fluctuations.push_back(highlow);
                }
                else{
                    fluctuations.push_back(highlow);
                    fluctuations.push_back(high);
                    fluctuations.push_back(highlow);
                }
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->fluctuate(fluctuations);
                }
                break;
            case MarketEventType::Recession:
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->updateFactors(factors*0.70);
                }
                break;
            case MarketEventType::Prosperity:
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->updateFactors(factors*1.30);
                }
                break;
            case MarketEventType::OtherGovPolicy:
                std::uniform_int_distribution<double> GEdistribution(0.0, 0.25);
                impact = GEdistribution(generator);
                factors = ME.applyGovImpact(factors, impact);
                for (auto& pair : stocks) { // directly apply fluctuations to stocks
                    pair.second->updateFactors(factors);
                }
                break;
            case MarketEventType::Nothing:
                break;
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
    }

    void reset(){
        traders.clear();
        stocks.clear();
        orderbook->clear();
    }
};