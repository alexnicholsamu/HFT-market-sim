#include <queue>
#include <vector>
#include <iostream>
#include <map>
#include <cmath>
#include <random>
#include <variant>

enum class MarketEventType { InterestRateChange, GlobalEconomy, EconomicIndicatorReports, 
    PublicOpinion, Recession, Prosperity, OtherGovPolicy };

class MarketEvent{
public: 
    MarketEventType type;
    double impact;
    double interestRate = 1.0;
    double factors = 1.0;
    MarketEvent(MarketEventType type, double impact);
    double applyInterestImpact(double interestRate, double factors);
    double interestFactor(double factor, double factors);
    std::vector<double> generateRandomChange();
};