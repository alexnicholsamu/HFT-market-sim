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
    Stock(std::string name, double price);
    double getPrice(std::mutex& mtx);
    void updateFactors(double factor, std::mutex& mtx);
    void fluctuate(std::vector<double> fluctuations, std::mutex& mtx);
    void editPrice(double amount, bool dir, std::mutex& mtx);
    void econIndicators(double factors, double impact, std::mutex& mtx);
};

#endif