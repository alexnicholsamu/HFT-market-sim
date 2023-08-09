#include "Stock.h"

Stock::Stock(std::string name, double price): name(name), price(price), rd(), generator(rd()) {}

double Stock::getPrice(){
    /*
        Helper method
    */
    return factors*price;
}

void Stock::updateFactors(double factor, std::mutex& factmtx){
    /*
        Updates stock prices via factor
    */
    std::lock_guard<std::mutex> lock(factmtx);
    factors = factor;
}

void Stock::fluctuate(std::vector<double> fluctuations, std::mutex& flucmtx){
    /*
        Alters stock prices via fluctuating it randomly. The factors of fluctuation can be changed, though the scale of the change
        can vastly differ by small changes. 
    */
    std::lock_guard<std::mutex> lock(flucmtx);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::atomic<double> stockCheck = distribution(generator);
    std::atomic<double> degreeCheck = distribution(generator);
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
}

void Stock::editPrice(double amount, bool dir){
    /*
        Helper function to have dirrect result on price
    */
    if(dir){
        price *= (1 + amount);
    }
    else{
        price *= (1 - amount);
    }
}

void Stock::econIndicators(double factors, double impact, std::mutex& econmtx){
    /*
        Market event helper function
    */
    std::uniform_real_distribution<double> distribution(0.0, factors*2);
    std::atomic<double> econReport = distribution(generator);
    std::atomic<double> affectStock = 0.5;
    if(econReport>affectStock){
        updateFactors(factors*(impact+econReport), econmtx);
    }
}