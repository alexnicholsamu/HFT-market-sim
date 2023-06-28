#include "Trade.h"

class Trade {
public:
    Order buyOrder;
    Order sellOrder;
    std::chrono::system_clock::time_point timestamp;
    int tradeQuantity;

    Trade(Order buyOrder, Order sellOrder): buyOrder(buyOrder), sellOrder(sellOrder),
      tradeQuantity(buyOrder.quantity < sellOrder.quantity ? buyOrder.quantity : sellOrder.quantity),
      timestamp(std::chrono::system_clock::now()) {}
};