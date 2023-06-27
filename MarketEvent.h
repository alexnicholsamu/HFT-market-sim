#include <queue>
#include <vector>
#include <iostream>
#include <map>

#include "Stock.h"

enum class MarketEventType { InterestRateChange, EarningsReport };

class MarketEvent{
public: 
    MarketEventType type;
    double impact;
    MarketEvent(MarketEventType type, double impact);
    void applyImpact();
};