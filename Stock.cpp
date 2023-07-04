#include "Stock.h"

Stock::Stock(std::string name, double price): name(name), price(price), rd(), generator(rd()) {}

double Stock::getPrice(std::mutex& mtx){
    std::lock_guard<std::mutex> lock(mtx);
    return factors*price;
}

void Stock::updateFactors(double factor, std::mutex& mtx){
    std::lock_guard<std::mutex> lock(mtx);
    factors = factor;
}

void Stock::fluctuate(std::vector<double> fluctuations, std::mutex& mtx){
    std::lock_guard<std::mutex> lock(mtx);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double stockCheck = distribution(generator);
    double degreeCheck = distribution(generator);
    std::cout << "fluctuation checkpoint 2" << std::endl;
    bool pos = true;
    if(stockCheck > (fluctuations[0]/3)){
        if(0.5 > fluctuations[1]){
            pos = false;
        }
        if(degreeCheck > fluctuations[2]){
            editPrice((degreeCheck/12), pos, mtx);
        }
        else{
            editPrice((degreeCheck/18), pos, mtx);
        }
    }
    std::cout << "Market Fluctuated!" << std::endl;
}

void Stock::editPrice(double amount, bool dir, std::mutex& mtx){
    std::lock_guard<std::mutex> lock(mtx);
    if(dir){
        price *= (1 + amount);
    }
    else{
        price *= (1 - amount);
    }
}

void Stock::econIndicators(double factors, double impact, std::mutex& mtx){
    std::lock_guard<std::mutex> lock(mtx);
    std::uniform_real_distribution<double> distribution(0.0, factors*2);
    double econReport = distribution(generator);
    double affectStock = 0.5;
    if(econReport>affectStock){
        updateFactors(factors*(impact+econReport), mtx);
    }
}