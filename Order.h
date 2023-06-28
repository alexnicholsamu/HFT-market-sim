#include <string>
#include <vector>
#include <map>

#include "Stock.h"

enum class OrderType { Buy, Sell };
enum class OrderStatus { Open, Partial, Closed, Cancelled };

class Order{
public:
    OrderType type;
    Stock stock;
    int quantity;
    OrderStatus status = OrderStatus::Open;
    int id;
public:
    Order(OrderType type, Stock stock, int quantity, int id);
};