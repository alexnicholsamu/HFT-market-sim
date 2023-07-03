#include "Order.h"

Order::Order(OrderType type, std::shared_ptr<Stock> stock, int quantity, int id, OrderPreference pref): type(type), 
    stock(stock), quantity(quantity), status(status), id(id), 
    order_price(stock->getPrice()),  // get the price at the time of order creation
    timestamp(std::chrono::system_clock::now()), pref(pref) {}
