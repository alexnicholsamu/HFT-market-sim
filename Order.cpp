#include "Order.h"

class Order{
public:
    OrderType type;
    Stock stock;
    int quantity;
    OrderStatus status;

    Order(OrderType type, Stock stock, int quantity, OrderStatus status): type(type), 
        stock(stock), quantity(quantity), status(status) {}
};