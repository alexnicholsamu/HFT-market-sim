#include <string>
#include <vector>
#include <map>

#include "Stock.h"

enum class OrderType { Buy, Sell };

class Order{
public:
    OrderType type;
    Stock stock;
    int quantity;
    std::string company;

public:
    Order(OrderType type, Stock stock, int quantity, std::string company);
};