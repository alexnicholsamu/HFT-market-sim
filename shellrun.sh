#!/bin/bash

# Compile the project. -std=c++17 is used to ensure you have the C++17 standard
# for std::shared_ptr and other features. -pthread is for multi-threading.
g++ -std=c++17 -pthread *.cpp -o simulation

# Run the resulting binary
./simulation
