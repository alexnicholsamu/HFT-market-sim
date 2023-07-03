#include <iostream>

#include "main.h"
#include "Market.h"

int main() {
    std::unique_ptr<Market> market = std::make_unique<Market>();
    market->run();  // running the simulation
    return 0;
}
