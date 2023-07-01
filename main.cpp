

// The 'enum' keyword is used to declare an enumeration, a distinct type
// consisting of a set of named constants called enumerators.
enum class OrderType { Buy, Sell };
enum class MarketEventType { InterestRateChange, EarningsReport };
class OrderBook;
class Order;
class Stock;
class Trade;
class MarketEvent;
class Portfolio;

#include <thread>
#include <iostream>

int main() {
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << std::endl;
}