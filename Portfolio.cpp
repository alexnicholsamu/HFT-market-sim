#include "Portfolio.h"

class Portfolio {
public: 
    std::map<Stock, int> holdings;
    double available_funds;

    Portfolio(double available_funds): available_funds(available_funds) {}

    double portfolioValue(){
        double totvalue = 0;
        for(const auto& pair : holdings) {
            int quantity = pair.second;
            Stock stock = pair.first;
            totvalue += quantity * stock.getPrice();
        }
        return totvalue;
    }

    double totalFunds(){
        return portfolioValue() + available_funds;
    }

    void makeChange(Order& order){
        if(order.type == OrderType::Buy){
            Stock company = order.stock;
            int quantity = order.quantity;
            double moneychange = company.getPrice() * quantity;
            available_funds -= moneychange;
            holdings[company] += quantity;
        }
        else{
            Stock company = order.stock;
            int quantity = order.quantity;
            // this if/else checks if the company is in the map, if it reaches the end then it is not in
            if (holdings.find(company) == holdings.end()) { 
                std::cout << "Error: Stock not in portfolio!\n";
                return;
            } 
            else {
                if(holdings[company] - quantity<0){
                    std::cout << "Warning: Only have " << holdings[company] << " shares of " << company.name <<
                    ". Instead, selling all available shares";
                    quantity = holdings[company];
                }
                double moneychange = company.getPrice() * quantity;
                holdings[company] -= quantity;
                if(holdings[company]==0){
                    holdings.extract(holdings.find(company));
                }
            }
        }
    }

    std::vector<std::string> listCompanies(){
        std::vector<std::string> listComps;
        for(const auto& pair : holdings) {
            Stock stock = pair.first;
            listComps.push_back(stock.name);
        }
        return listComps;
    }
};