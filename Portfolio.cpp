#include "Portfolio.h"

class Portfolio {
public: 
    std::map<std::shared_ptr<Stock>, int> holdings;  // changed to pointers
    std::mutex mtx;

    Portfolio(): holdings(holdings) {}

    double portfolioValue(){
        double totvalue = 0;
        for(const auto& pair : holdings) {
            int quantity = pair.second;
            std::shared_ptr<Stock> stock = pair.first;  // changed to pointer
            totvalue += quantity * stock->getPrice();  // use -> instead of .
        }
        return totvalue;
    }

    double totalFunds(double available_funds){
        return portfolioValue() + available_funds;
    }

    double makeChange(std::shared_ptr<Order> order, double cash){
        std::lock_guard<std::mutex> lock(mtx);
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
        return cash;
    }

    std::vector<std::string> listCompanies(){
        std::vector<std::string> listComps;
        for(const auto& pair : holdings) {
            std::shared_ptr<Stock> stock = pair.first;  // changed to pointer
            listComps.push_back(stock->name);  // use -> instead of .
        }
        return listComps;
    }
};
