#include "Trader.h"

class Trader{
public:
    Portfolio portfolio;
    int id;
    double available_cash;
    OrderBook* orderbook;

    Trader(int id, double available_cash, OrderBook* orderbook): portfolio(portfolio), id(id), 
        available_cash(available_cash), orderbook(orderbook) {}
    
    void makeOrder(OrderType type, Stock stock, int quantity, int id){
        Order order = Order(type, stock, quantity, id);
        if(order.type == OrderType::Buy){
            double moneychange = order.stock.getPrice() * quantity;
            if(moneychange <= available_cash){
                available_cash -= moneychange;
                orderbook->addBuyOrder(order);
            }
            else{
                std::cout << "Insufficient funds available. Funds needed for order: " 
                    << moneychange << " Available funds: " << available_cash << "\n";
            }
        }
        else{
            if (portfolio.holdings.find(order.stock) == portfolio.holdings.end()) { 
                std::cout << "Error: Stock not in portfolio!\n";
                return;
            } 
            else {
                if(portfolio.holdings[order.stock] < order.quantity){
                    std::cout << "Warning: Only have " << portfolio.holdings[stock] << " shares of " << order.stock.name <<
                    ". Instead, selling all available shares \n";
                    order.quantity = portfolio.holdings[order.stock];
                }
                portfolio.holdings[order.stock] - order.quantity;
                if(portfolio.holdings[order.stock]==0){
                    portfolio.holdings.extract(portfolio.holdings.find(order.stock));
                }
                orderbook->addSellOrder(order);
            }
        }
    }

    void cancelOrder(Order order){
        bool cancel = orderbook->cancelOrder(order);
        if(cancel && order.type == OrderType::Buy){
            available_cash += order.stock.getPrice() * order.quantity;
        }
    }

    void updatePortfolio(Order order){
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