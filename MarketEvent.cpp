#include "MarketEvent.h"

class MarketEvent {
public:
    MarketEventType type;
    double impact;
    double interestRate = 1.0;
    double factors = 1.0;

    // Constructor
    MarketEvent(MarketEventType type, double impact, double interestRate, double factors): type(type), impact(impact), 
        interestRate(interestRate), factors(factors) {}

    void applyImpact() {
        // This is a placeholder implementation. You would want to
        // implement this method based on how you want your market events
        // to impact the order book.
        switch(type) {
            case MarketEventType::InterestRateChange:
                double oldIR = interestRate;
                interestRate += impact;
                if(interestRate < 0.01){
                    std::cout << "Warning: Will not simulate negative interest rate. Adjusting interest rate to 0.0";
                    interestRate = 0.01;
                }
                if(interestRate > 20.0){
                    std::cout << "Warning: Will not simulate >20.0 interest rate. Adjusting interest rate to 20.0";
                    interestRate = 20.0;
                }
                double change = 1 - interestRate / oldIR;
                marketFactor(change); // edit value to accurately reflect 
                break;
            case MarketEventType::EarningsReport:
                // Manipulate orderBook based on the impact of EarningsReport event.
                break;
        }
    }

    void marketFactor(double factor){
        factors *= factor;
    }

    double getFactor(){
        return factors;
    }


};