#ifndef MAIN_H
#define MAIN_H

#include "Portfolio.h"
#include "Order.h"
#include "Trade.h"
#include "OrderBook.h"
#include "Stock.h"
#include "Trader.h"
#include "Market.h"

class Market;
class OrderBook;
enum class MarketEventType;
enum class OrderPreference;
enum class OrderType;
class Trader;
class Trade;
class Order;
class Portfolio;
class Stock;

int main();

#endif