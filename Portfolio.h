#include <queue>
#include <vector>
#include <iostream>
#include <map>

#include "stock.h"
#include "order.h"

class Portfolio {
public:
    double available_funds;
    std::map<Stock, int> holdings; 

public:
    Portfolio(double available_funds);
    double portfolioValue();
    double totalFunds();
    void makeChange(Order& order);
    std::vector<std::string> listCompanies();
};