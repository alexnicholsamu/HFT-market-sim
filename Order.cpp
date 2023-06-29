#include "Order.h"

class Order {
public:
    OrderType type;
    Stock* stock;  // changed to pointer
    double order_price;  // added this field to hold the static price at the time the order is created
    int quantity;
    OrderStatus status = OrderStatus::Open;
    int id;
    std::chrono::system_clock::time_point timestamp;

    Order(OrderType type, Stock* stock, int quantity, int id): type(type), 
        stock(stock), quantity(quantity), status(status), id(id), 
        order_price(stock->getPrice()),  // get the price at the time of order creation
        timestamp(std::chrono::system_clock::now()) {}
};
