#include "Trader.h"

Trader::Trader(int id, double available_cash, std::shared_ptr<OrderBook> orderbook): 
    id(id), available_cash(available_cash), orderbook(orderbook), rd(), generator(rd()) {}


void Trader::makeOrder(OrderType type, std::shared_ptr<Stock> stock, int quantity, OrderPreference pref, std::mutex& trademtx){
    /*
        Randomly generates order to be put in Orderbook. Care is taken to pre-process portfolio for the order (i.e. not double selling
        a stock)
    */
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
    /*
        Cancelling order, and applying impact of order. 
    */
    std::atomic<bool> cancel = orderbook->cancelOrder(order, trademtx);
    if(cancel && order->type == OrderType::Buy){
        available_cash += order->order_price * order->quantity;
        std::cout << "Order for Trader " << id << " Cancelled!" << std::endl; 
    }
    if(cancel && order->type == OrderType::Sell){
        portfolio.cancelSell(order);
        std::cout << "Order for Trader " << id << " Cancelled!" << std::endl; 
    }
}

void Trader::updatePortfolio(std::shared_ptr<Order> order, std::mutex& ordmtx){
    /*
        Helper function
    */
    available_cash = portfolio.makeChange(order, available_cash);
}

void Trader::doAction(std::vector<std::shared_ptr<Stock>> stocks, std::mutex& trademtx){
    /*
        Heart of the Trader threads. This randomly generates actions to do, from a Market/Raw buy, Market/Raw sell, and cancellation.
        Orders could be created and sent, and the portfolio has a lot of pre-processing to account for safety of synchonization and
        delayed Orderbook Execution. There is room for customization witihn the percentages/probability distribution, but a Raw buy
        is a necessary aspect of the trader options (unless, of course, they are initialzied with a portfolio already)
    */
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
    if(action < 0.22 && portfolio_size > 0){
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
    else if(action < 0.44 && portfolio_size > 0){
        std::uniform_int_distribution<int> stockSellDistribution(0, portfolio_size-1);
        choice = stockSellDistribution(generator);
        chosenStock = portfolio.listStocks()[choice];
        std::uniform_int_distribution<int> quantSellDistribution(1, portfolio.holdings[chosenStock]);
        quantitySell = quantSellDistribution(generator);
        portfolio.soldStock(chosenStock, quantitySell, available_cash);
    }
    else if(action < 0.70){
        choice = stockBuyDistribution(generator);
        chosenStock = stocks[choice];
        std::uniform_int_distribution<int> quantBuyDistribution(1,(int)floor(available_cash/(chosenStock->getPrice())));
        quantityBuy = quantBuyDistribution(generator);
        available_cash -= (chosenStock->getPrice() * quantityBuy);
        portfolio.boughtStock(chosenStock, quantityBuy);
    }
    else if(action < 0.96){
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
    else{
        std::uniform_int_distribution<int> orderCancelDistribution(0, active_orders.size()-1);
        choice = orderCancelDistribution(generator);
        if(!(active_orders.size() == 0)){
            cancelOrder(active_orders[choice], trademtx);
        }
    }
}
