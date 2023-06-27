#include "Stock.h"

class Stock {
public:
    std::string name;
    double price;
    double factors;

    Stock(std::string name, double price, double factors): name(name), price(price), factors(factors) {}

    void marketEffect(double factor){
        factors = factors * factor;
    }

    double getPrice(){
        return factors*price;
    }
};