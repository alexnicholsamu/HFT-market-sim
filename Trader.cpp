#include "Trader.h"

Trader::Trader(int id, double available_cash, std::shared_ptr<OrderBook> orderbook): 
    id(id), available_cash(available_cash), orderbook(orderbook), rd(), generator(rd()) {}


void Trader::makeOrder(OrderType type, std::shared_ptr<Stock> stock, int quantity, OrderPreference pref){
    std::shared_ptr<Order> order = std::make_shared<Order>(type, stock, quantity, id, pref);
    double order_price = order->order_price;  // use the price at the time of order creation
    if(order->type == OrderType::Buy){
        double moneychange = order_price * quantity;
        if(moneychange <= available_cash){
            available_cash -= moneychange;
            active_orders.push_back(order);
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
            active_orders.push_back(order);
            orderbook->addOrder(order);
        }
    }
}

void Trader::cancelOrder(std::shared_ptr<Order> order){
    bool cancel = orderbook->cancelOrder(order);
    if(cancel && order->type == OrderType::Buy){
        available_cash += order->order_price * order->quantity;  // use the price at the time of order creation
    }
    if(cancel && order->type == OrderType::Sell){
        portfolio.cancelSell(order);
    }
}

void Trader::updatePortfolio(std::shared_ptr<Order> order){
    available_cash = portfolio.makeChange(order, available_cash);
}

void Trader::doAction(std::vector<std::shared_ptr<Stock>> stocks){
    int portfolio_size = portfolio.holdings.size();
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double action = distribution(generator);
    std::uniform_int_distribution<int> stockBuyDistribution(0, stocks.size()-1);
    std::shared_ptr<Stock> chosenStock;
    std::uniform_int_distribution<int> quantDistribution(1, portfolio.holdings[chosenStock]);
    OrderPreference type;
    OrderType Otype;
    std::uniform_int_distribution<int> stockSellDistribution(0, portfolio_size-1);
    std::uniform_int_distribution<int> orderCancelDistribution(0, active_orders.size()-1);
    int quantityBuy;
    int typeAction = distribution(generator);
    int quantitySell;
    int choice;
    int order_size = active_orders.size();
    if(action < 0.475){
        choice = stockBuyDistribution(generator);
        chosenStock = stocks[choice];
        std::uniform_int_distribution<int> quantBuyDistribution(0,(int)floor(available_cash/chosenStock->getPrice()));
        quantityBuy = quantBuyDistribution(generator);
        if(typeAction < 0.5){
            type = OrderPreference::Limit;
        }
        else{
            type = OrderPreference::Market;
        }
        makeOrder(OrderType::Buy, chosenStock, quantityBuy, type);
    }
    if(action < 0.95){
        if(portfolio_size>0){
            choice = stockSellDistribution(generator);
            chosenStock = portfolio.listStocks()[choice];
            quantitySell = quantDistribution(generator);
            if(typeAction < 0.5){
                type = OrderPreference::Limit;
            }
            else{
                type = OrderPreference::Market;
            }
            makeOrder(OrderType::Sell, chosenStock, quantitySell, type);
        }
    }
    else{
        choice = orderCancelDistribution(generator);
        orderbook->cancelOrder(active_orders[choice]);
    }
    std::chrono::seconds sleepDuration(1);
    std::this_thread::sleep_for(sleepDuration);
}
