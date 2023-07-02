#include "MarketEvent.h"

class MarketEvent {
public:
    MarketEventType type;
    
    std::random_device rd;
    std::mt19937 generator;

    MarketEvent(MarketEventType type): type(type), rd(), generator(rd()) {}

    double applyInterestImpact(double interestRate, double factors, double impact) {
        double oldIR = interestRate;
        interestRate += impact;
        if(interestRate < 0.01){
            std::cout << "Warning: Will not simulate negative interest rate. Adjusting interest rate to 0.01\n";
            interestRate = 0.01;
        }
        if(interestRate > 20.0){
            std::cout << "Warning: Will not simulate > 20.0 interest rate. Adjusting interest rate to 20.0\n";
            interestRate = 20.0;
        }
        double change = interestRate / oldIR; 
        return interestFactor(change, factors);
    }

    double applyGlobalEconomy(double factor, double impact){
        if(factor < 1.0){
            return factor*(1-impact);
        }
        else{
            return factor*(1+impact);
        }
    }

    double interestFactor(double factor, double factors){ 
        if(abs(factor - 1) < 0.000001){
            return factors;
        }
        else if(factor < 1){
            factors *= (1 + (1 - factor));
        }
        else if(factor < 2){
            factors *= 1 - (1 - (log2(factor)/2)); 
        }
        else{
            factors *= ((1/log2(factor))/2);
        }
        return factors;
    }

    double applyGovImpact(double factor, double impact){
        if(factor < 1.0){
            return factor*(1+impact);
        }
        else{
            return factor*(1-impact);
        }
    }

    std::vector<double> generateRandomChange(){
        std::uniform_int_distribution<double> distribution(0.0, 1.0);
        double chooseStocks = distribution(generator);
        double chooseDirection = distribution(generator);
        double chooseDegree = distribution(generator);
        std::vector<double> fluctuation;
        fluctuation.push_back(chooseStocks);
        fluctuation.push_back(chooseDirection);
        fluctuation.push_back(chooseDegree);
        return fluctuation;
    }
};