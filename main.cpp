#include <iostream>

#include "main.h"
#include "Market.h"

int main() {
    std::cout << "Checkpoint 0.5" << std::endl;
    std::unique_ptr<Market> market = std::make_unique<Market>();
    std::cout << "Checkpoint 1";
    market->run();  // running the simulation
    return 0;
}
