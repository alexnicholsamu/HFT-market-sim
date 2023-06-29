#include "Portfolio.h"

class Portfolio {
public: 
    std::map<Stock*, int> holdings;  // changed to pointers

    Portfolio(): holdings(holdings) {}

    double portfolioValue(){
        double totvalue = 0;
        for(const auto& pair : holdings) {
            int quantity = pair.second;
            Stock* stock = pair.first;  // changed to pointer
            totvalue += quantity * stock->getPrice();  // use -> instead of .
        }
        return totvalue;
    }

    double totalFunds(double available_funds){
        return portfolioValue() + available_funds;
    }

    double makeChange(Order& order, double available_funds){
        if(order.type == OrderType::Buy){
            holdings[&(order.stock)] += order.quantity;  // use & to get address (pointer)
            std::cout << "Buy order complete!";
        }
        else{
            double moneychange = order.stock.getPrice() * order.quantity;
            available_funds += moneychange;
            std::cout << "Sell order complete!";
        }
        return available_funds;
    }

    std::vector<std::string> listCompanies(){
        std::vector<std::string> listComps;
        for(const auto& pair : holdings) {
            Stock* stock = pair.first;  // changed to pointer
            listComps.push_back(stock->name);  // use -> instead of .
        }
        return listComps;
    }
};
