#include "Trade.h"

Trade::Trade(std::shared_ptr<Order> buyOrder, std::shared_ptr<Order> sellOrder): buyOrder(buyOrder), sellOrder(sellOrder),
      tradeQuantity(buyOrder->quantity < sellOrder->quantity ? buyOrder->quantity : sellOrder->quantity) {}
