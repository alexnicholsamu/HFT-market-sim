#include <iostream>

#include "main.h"
#include "Market.h"

int main() {
    std::shared_ptr<Market> market = std::make_shared<Market>();
    
    market->run();  // running the simulation

    return 0;
}
