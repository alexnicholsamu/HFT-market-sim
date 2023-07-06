#include "Stock.h"

Stock::Stock(std::string name, double price): name(name), price(price), rd(), generator(rd()) {}

double Stock::getPrice(){
    return factors*price;
}

void Stock::updateFactors(double factor, std::mutex& factmtx){
    std::lock_guard<std::mutex> lock(factmtx);
    factors = factor;
}

void Stock::fluctuate(std::vector<double> fluctuations, std::mutex& flucmtx){
    std::lock_guard<std::mutex> lock(flucmtx);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::atomic<double> stockCheck = distribution(generator);
    std::atomic<double> degreeCheck = distribution(generator);
    std::cout << "fluctuation checkpoint 2" << std::endl;
    std::atomic<bool> pos = true;
    if(stockCheck > (fluctuations[0]/3)){
        if(0.5 > fluctuations[1]){
            pos = false;
        }
        if(degreeCheck > fluctuations[2]){
            editPrice((degreeCheck/12), pos);
        }
        else{
            editPrice((degreeCheck/18), pos);
        }
    }
    std::cout << "Market Fluctuated!" << std::endl;
}

void Stock::editPrice(double amount, bool dir){
    if(dir){
        price *= (1 + amount);
    }
    else{
        price *= (1 - amount);
    }
}

void Stock::econIndicators(double factors, double impact, std::mutex& econmtx){
    std::uniform_real_distribution<double> distribution(0.0, factors*2);
    std::atomic<double> econReport = distribution(generator);
    std::atomic<double> affectStock = 0.5;
    if(econReport>affectStock){
        updateFactors(factors*(impact+econReport), econmtx);
    }
}