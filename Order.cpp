#include "Order.h"

Order::Order(OrderType type, std::shared_ptr<Stock> stock, int quantity, int id, OrderPreference pref): type(type), 
    stock(stock), quantity(quantity), id(id), order_price(stock->getPrice()), 
    timestamp(std::chrono::system_clock::now()), pref(pref) {}
