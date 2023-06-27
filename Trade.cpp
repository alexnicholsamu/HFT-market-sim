#include "Trade.h"

class Trade {
public:
    Order buyOrder;
    Order sellOrder;
    // Constructor
    Trade(Order buyOrder, Order sellOrder): buyOrder(buyOrder), sellOrder(sellOrder) {}
};