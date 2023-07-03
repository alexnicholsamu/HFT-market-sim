#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <cmath>
#include <random>
#include <variant>

class Stock{
public:
    std::string name;
    double price;
    double factors = 1.0;
    std::random_device rd;
    std::mt19937 generator;
    std::mutex mtx;

public:
    Stock(std::string name, double price);
    double getPrice();
    void updateFactors(double factor);
    void fluctuate(std::vector<double> fluctuations);
    void editPrice(double amount, bool dir);
    void econIndicators(double factors, double impact);
};