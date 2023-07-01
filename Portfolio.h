#include <queue>
#include <vector>
#include <iostream>
#include <map>

#include "stock.h"
#include "order.h"


class Portfolio {
public:
    std::map<std::shared_ptr<Stock>, int> holdings; 

public:
    Portfolio();
    double portfolioValue();
    double totalFunds(double available_funds);
    double makeChange(std::shared_ptr<Order> order, double available_funds);
    std::vector<std::string> listCompanies();
};