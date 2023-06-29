#include "Trade.h"

class Trade {
public:
    const Order buyOrder;
    const Order sellOrder;
    std::chrono::system_clock::time_point timestamp;
    const int tradeQuantity;

    Trade(Order buyOrder, Order sellOrder): buyOrder(buyOrder), sellOrder(sellOrder),
      tradeQuantity(buyOrder.quantity < sellOrder.quantity ? buyOrder.quantity : sellOrder.quantity),
      timestamp(std::chrono::system_clock::now()) {}
};