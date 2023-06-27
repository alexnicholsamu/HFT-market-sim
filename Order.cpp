#include "Order.h"

class Order{
public:
    OrderType type;
    Stock stock;
    int quantity;

    Order(OrderType type, Stock stock, int quantity): type(type), stock(stock), quantity(quantity) {}
};