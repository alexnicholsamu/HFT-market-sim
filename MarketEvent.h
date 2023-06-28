#include <queue>
#include <vector>
#include <iostream>
#include <map>

enum class MarketEventType { InterestRateChange, EarningsReport };

class MarketEvent{
public: 
    MarketEventType type;
    double impact;
    double interestRate;
    double factors;
    MarketEvent(MarketEventType type, double impact, double interestRate, double factors);
    void applyImpact();
    double marketFactor();
    double getFactor();
};