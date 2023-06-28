#include <queue>
#include <vector>
#include <iostream>
#include <map>

enum class MarketEventType { InterestRateChange, EarningsReport };

class MarketEvent{
public: 
    MarketEventType type;
    double impact;
    double interestRate = 1.0;
    double factors = 1.0;
    MarketEvent(MarketEventType type, double impact);
    void applyImpact();
    void marketFactor();
    double getFactor();
};