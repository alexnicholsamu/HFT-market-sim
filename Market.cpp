#include "Market.h"

class Market{
public:
    std::vector<Trader> traders;
    std::shared_ptr<OrderBook> orderbook;
    double interestRate = 1.0;
    double factors = 1.0;
    std::mutex mtx;
    std::vector<std::shared_ptr<Stock>> stocks;
    std::random_device rd;
    std::mt19937 generator;

    Market(): rd(), generator(rd()) {}

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
                applyMarketImpact(pair.second);
                break;
            }
        }
        std::chrono::seconds sleepDuration(40);
        std::this_thread::sleep_for(sleepDuration);
    }

    void fluctuateMarket(){
        std::uniform_int_distribution<double> distribution(0.0, 1.0);
        double chooseStocks = distribution(generator);
        double chooseDirection = distribution(generator);
        double chooseDegree = distribution(generator);
        std::vector<double> marketFluctuations;
        marketFluctuations.push_back(chooseStocks);
        marketFluctuations.push_back(chooseDirection);
        marketFluctuations.push_back(chooseDegree);
        for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
            stock->fluctuate(marketFluctuations);
        }
        std::chrono::seconds sleepDuration(2);
        std::this_thread::sleep_for(sleepDuration);
    }

    void applyMarketImpact(MarketEventType ME){
        double impact;
        std::vector<double> fluctuations;
        switch(ME){
            case MarketEventType::InterestRateChange:
                std::uniform_int_distribution<double> interestDistribution(-4.0, 4.0);
                impact = interestDistribution(generator);
                interestRate += impact;
                if(interestRate < 0.01){
                    std::cout << "Warning: Will not simulate negative interest rate. Adjusting interest rate to 0.01\n";
                    interestRate = 0.01;
                }
                if(interestRate > 20.0){
                    std::cout << "Warning: Will not simulate > 20.0 interest rate. Adjusting interest rate to 20.0\n";
                    interestRate = 20.0;
                }
                double oldIR = interestRate;
                interestRate += impact;
                double change = interestRate / oldIR; 
                if(change < 1){
                    factors *= (1 + (1 - change));
                }
                else if(change < 2){
                    factors *= 1 - (1 - (log2(change)/2)); 
                }
                else{
                    factors *= ((1/log2(change))/2);
                }
                for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                    stock->updateFactors(factors);
                }
                break;
            case MarketEventType::GlobalEconomy:
                std::uniform_int_distribution<double> GEdistribution(0.0, 0.25);
                impact = GEdistribution(generator);
                if(factors < 1.0){
                    factors *= (1-impact);
                }
                else{
                    factors *= (1+impact);
                }
                for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                    stock->updateFactors(factors);
                }
                break;
            case MarketEventType::EconomicIndicatorReports:
                std::uniform_int_distribution<double> EIRdistribution(0.25, 0.50);
                impact = EIRdistribution(generator);
                for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                    stock->econIndicators(factors, impact);
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
                    fluctuations.push_back(highlow * 1.3);
                    fluctuations.push_back(low);
                    fluctuations.push_back(highlow * 0.4);
                }
                else{
                    fluctuations.push_back(highlow * 0.7);
                    fluctuations.push_back(high);
                    fluctuations.push_back(highlow * 0.4);
                }
                for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                    stock->fluctuate(fluctuations);
                }
                break;
            case MarketEventType::Recession:
                for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                    stock->updateFactors(factors*0.70);
                }
                break;
            case MarketEventType::Prosperity:
                for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                    stock->updateFactors(factors*1.30);
                }
                break;
            case MarketEventType::OtherGovPolicy:
                std::uniform_int_distribution<double> GEdistribution(0.0, 0.25);
                impact = GEdistribution(generator);
                if(factors < 1.0){
                    factors *= (1+impact);
                }
                else{
                     factors *= (1-impact);
                }
                for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                    stock->updateFactors(factors);
                }
                break;
            case MarketEventType::Nothing:
                break;
        }
    }

    void run(){
        bool running = true;
        initializeTraders("traders.txt", traders);
        initializeStocks("stocks.txt", stocks);
        while(running){
            
        }
    }

    void reset(){
        traders.clear();
        stocks.clear();
        orderbook->clear();
    }

    void initializeTraders(std::string filename, std::vector<Trader>& traders) {
        std::ifstream file(filename);
        int id;
        double cash;
        while (file >> id >> cash) {
            traders.push_back(Trader(id, cash, orderbook));
        }
        file.close();
    }

    void initializeStocks(std::string filename, std::vector<std::shared_ptr<Stock>>& stocks) {
        std::ifstream file(filename);
        std::string name;
        double price;
        while (file >> name >> price) {
            stocks.push_back(std::make_shared<Stock>(name, price));
        }
        file.close();
    }
};