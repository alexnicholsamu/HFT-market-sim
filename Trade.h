#include <queue>
#include <vector>
#include <iostream>
#include <map>

#include "Order.h"

class Trade{
public:
    Order buyOrder;
    Order sellOrder;
    Trade(Order buyOrder, Order sellOrder);
};