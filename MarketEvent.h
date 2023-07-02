#include <queue>
#include <vector>
#include <iostream>
#include <map>
#include <cmath>
#include <random>
#include <variant>

enum class MarketEventType { InterestRateChange, GlobalEconomy, EconomicIndicatorReports, 
    PublicOpinion, Recession, Prosperity, OtherGovPolicy, Nothing };

class MarketEvent{
public: 
    MarketEventType type;
    MarketEvent(MarketEventType type);
    double applyInterestImpact(double interestRate, double factors, double impact);
    double interestFactor(double factor, double factors);
    double applyGlobalEconomy(double factor, double impact);
    std::vector<double> generateRandomChange();
    double applyGovImpact(double factor, double impact);
};