#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>

#include "MarketEvent.h"

class Stock{
public:
    std::string name;
    double price;
    std::mutex mtx;

public:
    Stock(std::string name, double price);
    double getPrice();
    void updateFactors(double factor);
    void fluctuate(std::vector<double> fluctuations);
    void editPrice(double amount, bool dir);
};