#include "Stock.h"

Stock::Stock(std::string name, double price): name(name), price(price), rd(), generator(rd()) {}

double Stock::getPrice(){
    std::lock_guard<std::mutex> lock(getpmtx);
    return factors*price;
}

void Stock::updateFactors(double factor){
    std::lock_guard<std::mutex> lock(upfacmtx);
    factors = factor;
}

void Stock::fluctuate(std::vector<double> fluctuations){
    std::lock_guard<std::mutex> lock(fluctmtx);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double stockCheck = distribution(generator);
    double degreeCheck = distribution(generator);
    bool pos = true;
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
}

void Stock::editPrice(double amount, bool dir){
    std::lock_guard<std::mutex> lock(pricemtx);
    if(dir){
        price *= (1 + amount);
    }
    else{
        price *= (1 - amount);
    }
}

void Stock::econIndicators(double factors, double impact){
    std::lock_guard<std::mutex> lock(econmtx);
    std::uniform_real_distribution<double> distribution(0.0, factors*2);
    double econReport = distribution(generator);
    double affectStock = 0.5;
    if(econReport>affectStock){
        updateFactors(factors*(impact+econReport));
    }
}