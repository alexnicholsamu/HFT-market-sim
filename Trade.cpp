#include "Trade.h"

class Trade {
public:
    const std::shared_ptr<Order> buyOrder;
    const std::shared_ptr<Order> sellOrder;
    std::chrono::system_clock::time_point timestamp;
    const int tradeQuantity;

    Trade(std::shared_ptr<Order> buyOrder, std::shared_ptr<Order> sellOrder): buyOrder(buyOrder), sellOrder(sellOrder),
      tradeQuantity(buyOrder->quantity < sellOrder->quantity ? buyOrder->quantity : sellOrder->quantity),
      timestamp(std::chrono::system_clock::now()) {}
};