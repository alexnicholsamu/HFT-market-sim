#include "Stock.h"

class Stock {
public:
    std::string name;
    double price;
    double factors = 1.0;
    std::random_device rd;
    std::mt19937 generator;

    Stock(std::string name, double price): name(name), price(price), rd(), generator(rd()) {}

    double getPrice(){
        return factors*price;
    }

    void updateFactors(double factor){
        factors = factor;
    }

    void fluctuate(std::vector<double> fluctuations){
        std::uniform_int_distribution<double> distribution(0.0, 1.0);
        double stockCheck = distribution(generator);
        double degreeCheck = distribution(generator);
        bool pos = true;
        if(stockCheck > (fluctuations[0]/3)){
            if(0.5 < fluctuations[1]){
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

    void editPrice(double amount, bool dir){
        if(dir){
            price *= (1 + amount);
        }
        else{
            price *= (1 - amount);
        }
    }
};