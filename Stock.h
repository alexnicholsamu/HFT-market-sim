#ifndef STOCK_H
#define STOCK_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <random>
#include <variant>
#include <atomic>
#include <queue>
#include <chrono>
#include <thread>
#include <fstream>
#include <atomic>
#include <cmath>
#include <set>
#include <algorithm>
#include <mutex>

class Stock{
public:
    std::string name;
    double price;
    double factors = 1.0;
    std::random_device rd;
    std::mt19937 generator;
    std::mutex mtx;
    Stock(std::string name, double price);
    double getPrice();
    void updateFactors(double factor);
    void fluctuate(std::vector<double> fluctuations);
    void editPrice(double amount, bool dir);
    void econIndicators(double factors, double impact);
};

#endif