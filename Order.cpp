#include "Order.h"

Order::Order(OrderType type, std::shared_ptr<Stock> stock, int quantity, int id, OrderPreference pref, std::mutex& mtx): type(type), 
    stock(stock), quantity(quantity), status(status), id(id), 
    order_price(stock->getPrice(mtx)),  // get the price at the time of order creation
    timestamp(std::chrono::system_clock::now()), pref(pref) {}
