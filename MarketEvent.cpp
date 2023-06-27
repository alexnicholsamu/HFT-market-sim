#include "MarketEvent.h"

class MarketEvent {
public:
    MarketEventType type;
    double impact;
    double interestRate;
    // Constructor
    MarketEvent(MarketEventType type, double impact, double interestRate): type(type), impact(impact), 
        interestRate(interestRate) {}

    void applyImpact() {
        // This is a placeholder implementation. You would want to
        // implement this method based on how you want your market events
        // to impact the order book.
        switch(type) {
            case MarketEventType::InterestRateChange:
                double change = interestRate-impact/interestRate;
                if(change>0){
                    
                }
                Stock.marketEffect(double effect); // edit value to accurately reflect 
                break;
            case MarketEventType::EarningsReport:
                // Manipulate orderBook based on the impact of EarningsReport event.
                break;
        }
    }
};