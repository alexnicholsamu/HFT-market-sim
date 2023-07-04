#include "Trader.h"

Trader::Trader(int id, double available_cash, std::shared_ptr<OrderBook> orderbook): 
    id(id), available_cash(available_cash), orderbook(orderbook), rd(), generator(rd()) {}


void Trader::makeOrder(OrderType type, std::shared_ptr<Stock> stock, int quantity, OrderPreference pref, std::mutex& mtx){
    std::shared_ptr<Order> order = std::make_shared<Order>(type, stock, quantity, id, pref, mtx);
    double order_price = order->order_price; 
    if(order->type == OrderType::Buy){
        double moneychange = order_price * quantity;
        available_cash -= moneychange;
        active_orders.push_back(order);
        orderbook->addOrder(order, mtx);
    }
    else{
        portfolio.holdings[stock] -= quantity;
        if(portfolio.holdings[stock]==0){
            portfolio.holdings.erase(stock);
        }
        active_orders.push_back(order);
        orderbook->addOrder(order, mtx);
    }
}


void Trader::cancelOrder(std::shared_ptr<Order> order, std::mutex& mtx){
    bool cancel = orderbook->cancelOrder(order, mtx);
    if(cancel && order->type == OrderType::Buy){
        available_cash += order->order_price * order->quantity;
        std::cout << "Order Canceled!\n" << std::endl; 
    }
    if(cancel && order->type == OrderType::Sell){
        portfolio.cancelSell(order, mtx);
        std::cout << "Order Canceled!\n" << std::endl; 
    }
}

void Trader::updatePortfolio(std::shared_ptr<Order> order, std::mutex& mtx){
    available_cash = portfolio.makeChange(order, available_cash, mtx);
}

void Trader::doAction(std::vector<std::shared_ptr<Stock>> stocks, std::mutex& mtx){
    int portfolio_size = portfolio.holdings.size();
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double action = distribution(generator);
    std::uniform_int_distribution<int> stockBuyDistribution(0, stocks.size()-1);
    std::shared_ptr<Stock> chosenStock;
    std::uniform_int_distribution<int> quantSellDistribution(1, portfolio.holdings[chosenStock]);
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
        std::uniform_int_distribution<int> quantBuyDistribution(0,(int)floor(available_cash/chosenStock->getPrice(mtx)));
        quantityBuy = quantBuyDistribution(generator);
        if(typeAction < 0.5){
            type = OrderPreference::Limit;
        }
        else{
            type = OrderPreference::Market;
        }
        makeOrder(OrderType::Buy, chosenStock, quantityBuy, type, mtx);
    }
    if(action < 0.95){
        if(portfolio_size>0){
            choice = stockSellDistribution(generator);
            chosenStock = portfolio.listStocks(mtx)[choice];
            quantitySell = quantSellDistribution(generator);
            if(typeAction < 0.5){
                type = OrderPreference::Limit;
            }
            else{
                type = OrderPreference::Market;
            }
            makeOrder(OrderType::Sell, chosenStock, quantitySell, type, mtx);
        }
    }
    else{
        choice = orderCancelDistribution(generator);
        cancelOrder(active_orders[choice], mtx);
    }
    std::chrono::seconds sleepDuration(1);
    std::this_thread::sleep_for(sleepDuration);
}
