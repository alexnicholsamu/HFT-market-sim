#include "Trader.h"

class Trader{
public:
    Portfolio portfolio;
    int id;
    double available_cash;
    std::shared_ptr<OrderBook> orderbook;

    Trader(int id, double available_cash, std::shared_ptr<OrderBook> orderbook): 
        id(id), available_cash(available_cash), orderbook(orderbook) {}

    
    void makeOrder(OrderType type, std::shared_ptr<Stock> stock, int quantity, int id, OrderPreference pref){
        std::shared_ptr<Order> order = std::make_shared<Order>(type, stock, quantity, id, pref);
        double order_price = order->order_price;  // use the price at the time of order creation
        if(order->type == OrderType::Buy){
            double moneychange = order_price * quantity;
            if(moneychange <= available_cash){
                available_cash -= moneychange;
                orderbook->addOrder(order);
            }
            else{
                std::cout << "Insufficient funds available. Funds needed for order: " 
                    << moneychange << " Available funds: " << available_cash << "\n";
            }
        }
        else{
            if (portfolio.holdings.find(stock) == portfolio.holdings.end()) { 
                std::cout << "Error: Stock not in portfolio!\n";
                return;
            } 
            else {
                if(portfolio.holdings[stock] < quantity){
                    std::cout << "Warning: Only have " << portfolio.holdings[stock] << " shares of " << stock->name <<
                    ". Instead, selling all available shares \n";
                    quantity = portfolio.holdings[stock];
                }
                portfolio.holdings[stock] -= quantity;
                if(portfolio.holdings[stock]==0){
                    portfolio.holdings.erase(stock);
                }
                orderbook->addOrder(order);
            }
        }
    }

    void cancelOrder(std::shared_ptr<Order> order){
        bool cancel = orderbook->cancelOrder(order);
        if(cancel && order->type == OrderType::Buy){
            available_cash += order->order_price * order->quantity;  // use the price at the time of order creation
        }
    }

    void updatePortfolio(std::shared_ptr<Order> order){
        available_cash = portfolio.makeChange(order, available_cash);
    }

    void getPositions(){
        std::vector<std::string> positions = portfolio.listCompanies();
        for(std::string position : positions){
            std::cout << position;
        }
    }

    void getPortfolioValue(){
        std::cout << portfolio.portfolioValue();
    }

    void getTotalValue(){
        std::cout << portfolio.totalFunds(available_cash);
    }

};
