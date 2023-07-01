#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <memory>

#include "Stock.h"

enum class OrderType { Buy, Sell };
enum class OrderStatus { Open, Partial, Closed, Cancelled };

class Order{
public:
    OrderType type;
    std::shared_ptr<Stock> stock;
    int quantity;
    OrderStatus status = OrderStatus::Open;
    int id;
    double order_price;
    std::chrono::system_clock::time_point timestamp;
public:
    Order(OrderType type, std::shared_ptr<Stock> stock, int quantity, int id);
};