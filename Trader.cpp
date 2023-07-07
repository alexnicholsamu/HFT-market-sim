#include "Trader.h"

Trader::Trader(int id, double available_cash, std::shared_ptr<OrderBook> orderbook): 
    id(id), available_cash(available_cash), orderbook(orderbook), rd(), generator(rd()) {}


void Trader::makeOrder(OrderType type, std::shared_ptr<Stock> stock, int quantity, OrderPreference pref, std::mutex& trademtx){
    std::shared_ptr<Order> order = std::make_shared<Order>(type, stock, quantity, id, pref);
    std::atomic<double> order_price = order->order_price; 
    if(order->type == OrderType::Buy){
        std::atomic<double> moneychange = order_price * quantity;
        available_cash -= moneychange;
        active_orders.push_back(order);
        orderbook->addOrder(order, trademtx);
    }
    else{
        portfolio.holdings[stock] -= quantity;
        if(portfolio.holdings[stock]==0){
            portfolio.holdings.erase(stock);
        }
        active_orders.push_back(order);
        orderbook->addOrder(order, trademtx);
    }
}


void Trader::cancelOrder(std::shared_ptr<Order> order, std::mutex& trademtx){
    std::atomic<bool> cancel = orderbook->cancelOrder(order, trademtx);
    if(cancel && order->type == OrderType::Buy){
        available_cash += order->order_price * order->quantity;
        std::cout << "Order for Trader " << id << " Canceled!" << std::endl; 
    }
    if(cancel && order->type == OrderType::Sell){
        portfolio.cancelSell(order);
        std::cout << "Order for Trader " << id << " Canceled!" << std::endl; 
    }
}

void Trader::updatePortfolio(std::shared_ptr<Order> order, std::mutex& ordmtx){
    available_cash = portfolio.makeChange(order, available_cash);
}

void Trader::doAction(std::vector<std::shared_ptr<Stock>> stocks, std::mutex& trademtx){
    std::chrono::seconds sleepDuration(3);
    std::this_thread::sleep_for(sleepDuration);
    int portfolio_size = portfolio.holdings.size();
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double action = distribution(generator);
    std::uniform_int_distribution<int> stockBuyDistribution(0, stocks.size()-1);
    std::shared_ptr<Stock> chosenStock;
    OrderPreference type;
    OrderType Otype;
    int quantityBuy;
    double typeAction = distribution(generator);
    int quantitySell;
    int choice;
    int order_size = active_orders.size();
    if(action < 0.26){
        choice = stockBuyDistribution(generator);
        chosenStock = stocks[choice];
        std::uniform_int_distribution<int> quantBuyDistribution(1,(int)floor(available_cash/(chosenStock->getPrice())));
        quantityBuy = quantBuyDistribution(generator);
        available_cash -= (chosenStock->getPrice() * quantityBuy);
        portfolio.boughtStock(chosenStock, quantityBuy);
    }
    if(action < 0.48){
        if(portfolio_size>0){
            std::uniform_int_distribution<int> stockSellDistribution(0, portfolio_size-1);
            choice = stockSellDistribution(generator);
            chosenStock = portfolio.listStocks()[choice];
            std::uniform_int_distribution<int> quantSellDistribution(1, portfolio.holdings[chosenStock]);
            quantitySell = quantSellDistribution(generator);
            portfolio.soldStock(chosenStock, quantitySell, available_cash);
        }
    }
    else if(action < 0.74){
        choice = stockBuyDistribution(generator);
        chosenStock = stocks[choice];
        std::uniform_int_distribution<int> quantBuyDistribution(1,(int)floor(available_cash/(chosenStock->getPrice())));
        quantityBuy = quantBuyDistribution(generator);
        if(typeAction < 0.5){
            type = OrderPreference::Limit;
        }
        else{
            type = OrderPreference::Market;
        }
        makeOrder(OrderType::Buy, chosenStock, quantityBuy, type, trademtx);
    }
    else if(action < 0.96){
        if(portfolio_size>0){
            std::uniform_int_distribution<int> stockSellDistribution(0, portfolio_size-1);
            choice = stockSellDistribution(generator);
            chosenStock = portfolio.listStocks()[choice];
            std::uniform_int_distribution<int> quantSellDistribution(1, portfolio.holdings[chosenStock]);
            quantitySell = quantSellDistribution(generator);
            if(typeAction < 0.5){
                type = OrderPreference::Limit;
            }
            else{
                type = OrderPreference::Market;
            }
            makeOrder(OrderType::Sell, chosenStock, quantitySell, type, trademtx);
        }
    }
    else{
        std::uniform_int_distribution<int> orderCancelDistribution(0, active_orders.size()-1);
        choice = orderCancelDistribution(generator);
        if(!(active_orders.size() == 0)){
            cancelOrder(active_orders[choice], trademtx);
        }
    }
}
