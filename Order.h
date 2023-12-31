#ifndef ORDER_H
#define ORDER_H

#include "Stock.h"

enum class OrderType { Buy, Sell };
enum class OrderStatus { Open, Partial, Closed, Cancelled };
enum class OrderPreference { Limit, Market};

class Order{
public:
    OrderType type;
    std::shared_ptr<Stock> stock;  // changed to pointer
    double order_price;  // added this field to hold the static price at the time the order is created
    int quantity;
    OrderStatus status = OrderStatus::Open;
    int id;
    OrderPreference pref;

    Order(OrderType type, std::shared_ptr<Stock> stock, int quantity, int id, OrderPreference pref);
};

#endif 