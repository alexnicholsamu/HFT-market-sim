#include "Market.h"

Market::Market(): rd(), generator(rd()) {}

void Market::executeOrderBook(std::mutex& ordmtx){
    /*
        Where the orderbook is executed, and applied. If one is interested in customizing this code (proceed with caution), they should 
        look at the Orderbook Class or the Trader Class instead.
    */
    std::chrono::seconds sleepDuration(6);
    std::this_thread::sleep_for(sleepDuration);
    std::vector<std::shared_ptr<Order>> orders = orderbook->executeTrades(ordmtx);
    if(!orders.empty()){
        for(std::shared_ptr<Order> order : orders){ // I feel like there is room for efficiency improvements here
            for(std::shared_ptr<Trader>& trader : traders){
                if(trader->id == order->id){
                    trader->updatePortfolio(order, ordmtx);
                }
                if(trader->id == order->id){
                    trader->updatePortfolio(order, ordmtx);
                }
            }
        }
        std::cout << "OrderBook executed!" << std::endl;
    }
}

void Market::generateMarketEvent(std::map<double, MarketEventType> MEcreation, std::mutex& meventmtx){
    /*
        Where market events are randomly generated, and applied to the table. 
    */
    std::chrono::seconds sleepDuration(8);
    std::this_thread::sleep_for(sleepDuration);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double marketEvent = distribution(generator);
    for(auto& pair : MEcreation){
        if (marketEvent <= pair.first) {
            applyMarketImpact(pair.second, meventmtx);
            break;
        }
    }
}

void Market::fluctuateMarket(std::mutex& flucmtx){
    /*
        Where prices are continually altered to reflect random (though obivously not random, it is an appropiate astraction for the sake
        of this project) movements of the market. If one is interested in changes the scale of the impact / the threshold, they should
        look in the stock class
    */
    std::chrono::seconds sleepDuration(2);
    std::this_thread::sleep_for(sleepDuration);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double chooseStocks = distribution(generator);
    double chooseDirection = distribution(generator);
    double chooseDegree = distribution(generator);
    std::vector<double> marketFluctuations;
    marketFluctuations.push_back(chooseStocks);
    marketFluctuations.push_back(chooseDirection);
    marketFluctuations.push_back(chooseDegree);
    for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
        stock->fluctuate(marketFluctuations, flucmtx);
    }
    std::cout << "Market Fluctuated!" << std::endl;
}

void Market::applyMarketImpact(MarketEventType ME, std::mutex& meventmtx){
    /*
        Where market impacts are applied. The numbers in here were included based on mathematical probabilities / real world numbers, 
        so while they can be messed with proceed with caution as varying a few numbers can drastically change the scale of their impact.
    */
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
                stock->updateFactors(factors, meventmtx);
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
                stock->updateFactors(factors, meventmtx);
            }
            break;
        }
        case MarketEventType::EconomicIndicatorReports: {
            std::cout << "Market Event: Economic Indicator Report!" << std::endl;
            std::uniform_real_distribution<double> EIRdistribution(0.25, 0.50);
            impact = EIRdistribution(generator);
            for (std::shared_ptr<Stock>& stock : stocks) { // directly apply fluctuations to stocks
                stock->econIndicators(factors, impact, meventmtx);
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
                stock->fluctuate(fluctuations, meventmtx);
            }
            break;
        }
        case MarketEventType::Recession: {
            std::cout << "Market Event: Recession!" << std::endl;
            for (std::shared_ptr<Stock>& stock : stocks) { 
                stock->updateFactors(factors*0.70, meventmtx);
            }
            break;
        }
        case MarketEventType::Prosperity: {
            std::cout << "Market Event: Prosperity!" << std::endl;
            for (std::shared_ptr<Stock>& stock : stocks) { 
                stock->updateFactors(factors*1.30, meventmtx);
            }
            break;
        }
        case MarketEventType::OtherGovPolicy: {
            std::cout << "Market Event: Other Government Policy!" << std::endl;
            std::uniform_real_distribution<double> GEdistribution(0.0, 0.25);
            impact = GEdistribution(generator);
            if(factors < 1.0){
                factors *= (1+impact);
            }
            else{
                    factors *= (1-impact);
            }
            for (std::shared_ptr<Stock>& stock : stocks) { 
                stock->updateFactors(factors, meventmtx);
            }
            break;
        }
        case MarketEventType::Nothing:
            std::cout << "Market Event: Nothing!" << std::endl;
            break;
    }
}

std::map<double,MarketEventType> Market::generateMarketEventChances(){
    /*
        A customizable table of probability thresholds for Market Events. While there is certainly room to add marketevents, that would
        require a non-trivial modification of the code, so proceed with caution. Otherwise, the table thresholds can be edited to reflect
        a different average quantity of different events
    */
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
    /*
        Heart of the simulation. This creates my Traders/stocks, as well as generating my market event chance table.
        Then, it starts the threads, having one for each of the 25 (the amount I have) Traders, one that generates Market Events, 
        one that fluctuates stock prices randomly, and one that executes the orderbook. There is also the main thread that runs the 
        simulation for 5 minutes (300 seconds) - a customizable quantity - before joining the threads and wiping the vectors and 
        such clean.
    */
    std::cout << "Setting up simulation!"<< std::endl;
    std::atomic<bool> running(true);
    initializeTraders("traders.txt");
    initializeStocks("stocks.txt");
    std::map<double,MarketEventType> marketEventChance = generateMarketEventChances();
    std::vector<std::thread> threads;

    auto start = std::chrono::steady_clock::now();

    std::cout << "Starting Simulation!" << std::endl;

    std::thread METhread([this, &start, &running, marketEventChance] {
        while (running) {
            generateMarketEvent(marketEventChance, meventmtx);
        }
    });

    std::thread fluctuateThread([this, &start, &running] {
        while (running) {
            fluctuateMarket(flucmtx);
        }
    });

    std::thread orderbookThread([this, &start, &running] {
        while (running) {
            executeOrderBook(ordmtx);
        }
    });

    threads.push_back(std::move(METhread));
    threads.push_back(std::move(fluctuateThread));
    threads.push_back(std::move(orderbookThread));

    for (std::shared_ptr<Trader>& trader : traders) {
        std::thread traderThread([this, &start, &running, &trader] {
            while (running) {
                trader->doAction(stocks, trademtx);
            }
        });
        threads.push_back(std::move(traderThread));
    }

    std::this_thread::sleep_for(std::chrono::seconds(300));
    running = false;

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
    /*
        Cleanup. Most pointers should destruct automatically as they are shared/unique.
    */
    traders.clear();
    stocks.clear();
    orderbook->clear();
}

void Market::initializeTraders(std::string filename) {
    /*
        This code initializes my traders via my manually created traders.txt file. This file can be edited to use different traders with
        different initial money, though if the amount is too low their buying ability will be limited. Also, too many traders and there
        will be more significant overhead, as in most (laptop) systems only ~10 threads at a time are supported (I currently run 29)
    */
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
    /*
        This code initializes my stocks via my manually created stocks.txt file. This file can be edited to use 
        different stocks/different prices. It supports fake stocks/prices, just be wary of Trader money as if their
        price is too high, traders will not be able to buy them (unless they sell their way up to being able to buy)
    */
    std::ifstream file(filename);
    std::string name;
    double price;
    while (file >> name >> price) {
        stocks.push_back(std::make_shared<Stock>(name, price));
    }
    file.close();
    std::cout << "Initializing Stocks!" << std::endl;
}