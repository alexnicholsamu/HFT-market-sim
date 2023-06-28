#include "Order.h"

class Order{
public:
    OrderType type;
    Stock stock;
    int quantity;
    OrderStatus status = OrderStatus::Open;
    int id;
    std::chrono::system_clock::time_point timestamp;

    Order(OrderType type, Stock stock, int quantity, int id): type(type), 
        stock(stock), quantity(quantity), status(status), id(id), timestamp(std::chrono::system_clock::now()) {}
};