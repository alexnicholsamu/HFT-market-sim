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

    double makeChange(Order* order, double cash){
        if(order->type == OrderType::Buy){
            if(holdings.find(order->stock) != holdings.end()){
                holdings[order->stock] += order->quantity;
            }
            else{
                holdings[order->stock] = order->quantity;
            }
        }
        else{
            holdings[order->stock] -= order->quantity;
            if(holdings[order->stock] == 0){
                holdings.erase(order->stock);
            }
        }
        delete order;
        return cash;
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
