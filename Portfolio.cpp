#include "Portfolio.h"

class Portfolio {
public: 
    std::map<std::shared_ptr<Stock>, int> holdings;  // changed to pointers
    std::mutex mtx;

    Portfolio() {}

    double portfolioValue(){
        double totvalue = 0;
        for(const auto& pair : holdings) {
            int quantity = pair.second;
            std::shared_ptr<Stock> stock = pair.first;  // changed to pointer
            totvalue += quantity * stock->getPrice();  // use -> instead of .
        }
        return totvalue;
    }

    double makeChange(std::shared_ptr<Order> order, double cash){
        std::lock_guard<std::mutex> lock(mtx);
        if(order->type == OrderType::Buy){
            holdings[order->stock] += order->quantity;
        }
        else{
            holdings[order->stock] -= order->quantity;
            cash += order-> order_price * order->quantity;
            if(holdings[order->stock] == 0){
                holdings.erase(order->stock);
            }
        }
        return cash;
    }

    void cancelSell(std::shared_ptr<Order> order){
        holdings[order->stock] += order->quantity;
    }

    std::vector<std::shared_ptr<Stock>> listStocks(){
        std::vector<std::shared_ptr<Stock>> listComps;
        for(const auto& pair : holdings) {
            std::shared_ptr<Stock> stock = pair.first;  // changed to pointer
            listComps.push_back(stock);  // use -> instead of .
        }
        return listComps;
    }
};
