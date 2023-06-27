#include <string>
#include <vector>
#include <map>

class Stock{
public:
    std::string name;
    double price;
    double factors;

public:
    Stock(std::string name, double price, double factors);
    void marketEffect(double factor);
    double getPrice();
};