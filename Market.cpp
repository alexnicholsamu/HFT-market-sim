#include "Market.h"

Market::Market(): rd(), generator(rd()) {}

void Market::executeOrderBook(std::mutex& mtx){
    std::vector<std::shared_ptr<Order>> orders = orderbook->executeTrades(mtx);
    if(!orders.empty()){
        std::shared_ptr<Order> buyOrder = orders[0];
        std::shared_ptr<Order> sellOrder = orders[1];
        for(std::shared_ptr<Trader>& trader : traders){
            if(trader->id == buyOrder->id){
                trader->updatePortfolio(buyOrder, mtx);
            }
            if(trader->id == sellOrder->id){
                trader->updatePortfolio(sellOrder, mtx);
            }
        }
    }
}

void Market::generateMarketEvent(std::map<double, MarketEventType> MEcreation, std::mutex& mtx){
    std::chrono::seconds sleepDuration(10);
    std::this_thread::sleep_for(sleepDuration);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double marketEvent = distribution(generator);
    for(auto& pair : MEcreation){
        if (marketEvent <= pair.first) {
            applyMarketImpact(pair.second, mtx);
            break;
        }
    }
    
}

void Market::fluctuateMarket(std::mutex& mtx){
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double chooseStocks = distribution(generator);
    double chooseDirection = distribution(generator);
    double chooseDegree = distribution(generator);
    std::vector<double> marketFluctuations;
    marketFluctuations.push_back(chooseStocks);
    marketFluctuations.push_back(chooseDirection);
    marketFluctuations.push_back(chooseDegree);
    for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
        stock->fluctuate(marketFluctuations, mtx);
    }
    std::cout << "Market Fluctuated!\n" << std::endl;
    std::chrono::seconds sleepDuration(2);
    std::this_thread::sleep_for(sleepDuration);
}

void Market::applyMarketImpact(MarketEventType ME, std::mutex& mtx){
    double impact;
    std::vector<double> fluctuations;
    switch(ME){
        case MarketEventType::InterestRateChange: {
            std::cout << "Market Event: Interest Rate Change!" << std::endl;
            std::uniform_real_distribution<double> interestDistribution(-4.0, 4.0);
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
                stock->updateFactors(factors, mtx);
            }
            break;
        }
        case MarketEventType::GlobalEconomy: {
            std::cout << "Market Event: Global Economy!" << std::endl;
            std::uniform_real_distribution<double> GEdistribution(0.0, 0.25);
            impact = GEdistribution(generator);
            if(factors < 1.0){
                factors *= (1-impact);
            }
            else{
                factors *= (1+impact);
            }
            for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                stock->updateFactors(factors, mtx);
            }
            break;
        }
        case MarketEventType::EconomicIndicatorReports: {
            std::cout << "Market Event: Economic Indicator Report!" << std::endl;
            std::uniform_real_distribution<double> EIRdistribution(0.25, 0.50);
            impact = EIRdistribution(generator);
            for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                stock->econIndicators(factors, impact, mtx);
            }
            break;
        }
        case MarketEventType::PublicOpinion: {
            std::cout << "Market Event: Public Opinion!" << std::endl;
            std::uniform_real_distribution<double> lowdistribution(0.00, 0.60);
            std::uniform_real_distribution<double> highdistribution(0.40, 1.0);
            std::uniform_real_distribution<double> distribution(0.00, 1.0);
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
            for (std::shared_ptr<Stock>& stock : stocks) {
                stock->fluctuate(fluctuations, mtx);
            }
            break;
        }
        case MarketEventType::Recession: {
            std::cout << "Market Event: Recession!" << std::endl;
            for (std::shared_ptr<Stock>& stock : stocks) { 
                stock->updateFactors(factors*0.70, mtx);
            }
            break;
        }
        case MarketEventType::Prosperity: {
            std::cout << "Market Event: Prosperity!" << std::endl;
            for (std::shared_ptr<Stock>& stock : stocks) { 
                stock->updateFactors(factors*1.30, mtx);
            }
            break;
        }
        case MarketEventType::OtherGovPolicy: {
            std::cout << "Market Event: Other Government Policty!" << std::endl;
            std::uniform_real_distribution<double> GEdistribution(0.0, 0.25);
            impact = GEdistribution(generator);
            if(factors < 1.0){
                factors *= (1+impact);
            }
            else{
                    factors *= (1-impact);
            }
            for (std::shared_ptr<Stock>& stock : stocks) { 
                stock->updateFactors(factors, mtx);
            }
            break;
        }
        case MarketEventType::Nothing:
            std::cout << "Market Event: Nothing!" << std::endl;
            break;
    }
}

std::map<double,MarketEventType> Market::generateMarketEventChances(){
    std::map<double,MarketEventType> marketEventChance;
    marketEventChance[0.05] = MarketEventType::Prosperity;
    marketEventChance[0.10] = MarketEventType::Recession;
    marketEventChance[0.20] = MarketEventType::EconomicIndicatorReports;
    marketEventChance[0.30] = MarketEventType::GlobalEconomy;
    marketEventChance[0.40] = MarketEventType::InterestRateChange;
    marketEventChance[0.50] = MarketEventType::OtherGovPolicy;
    marketEventChance[0.60] = MarketEventType::PublicOpinion;
    marketEventChance[1.0] = MarketEventType::Nothing;
    return marketEventChance;
}

void Market::run(){
    std::cout << "Setting up simulation!"<< std::endl;
    std::atomic<bool> running(true);
    initializeTraders("traders.txt");
    initializeStocks("stocks.txt");
    std::map<double,MarketEventType> marketEventChance = generateMarketEventChances();
    std::vector<std::thread> threads;
    std::lock_guard<std::mutex> guard(mtx);

    auto start = std::chrono::steady_clock::now();

    std::cout << "Starting Simulation!" << std::endl;

    std::thread METhread([this, &start, &running, marketEventChance] {
        while (running) {
            generateMarketEvent(marketEventChance, mtx);
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= 300) {
                running = false;
            }
        }
    });

    std::thread fluctuateThread([this, &start, &running] {
        while (running) {
            fluctuateMarket(mtx);
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= 300) {
                running = false;
            }
        }
    });

    std::thread orderbookThread([this, &start, &running] {
        while (running) {
            executeOrderBook(mtx);
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= 300) {
                running = false;
            }
        }
    });

    threads.push_back(std::move(METhread));
    threads.push_back(std::move(fluctuateThread));
    threads.push_back(std::move(orderbookThread));

    for (std::shared_ptr<Trader>& trader : traders) {
        std::thread traderThread([this, &start, &running, &trader] {
            while (running) {
                trader->doAction(stocks, mtx);
                auto now = std::chrono::steady_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= 300) {
                    running = false;
                }
            }
        });
        threads.push_back(std::move(traderThread));
    }

    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    reset();
    std::cout << "Simulation Ended!" << std::endl;
    return;
}


void Market::reset(){
    traders.clear();
    stocks.clear();
    orderbook->clear();
}

void Market::initializeTraders(std::string filename) {
    std::ifstream file(filename);
    int id;
    double cash;
    while (file >> id >> cash) {
        traders.push_back(std::make_shared<Trader>(id, cash, orderbook));
    }
    file.close();
    std::cout << "Initializing Traders!" << std::endl;
}

void Market::initializeStocks(std::string filename) {
    std::ifstream file(filename);
    std::string name;
    double price;
    while (file >> name >> price) {
        stocks.push_back(std::make_shared<Stock>(name, price));
    }
    file.close();
    std::cout << "Initializing Stocks!" << std::endl;
}