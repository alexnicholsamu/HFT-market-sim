#!/bin/bash

echo "Deleting old main.exe, if it exists"
rm -f main.exe

echo "Starting build, may take a little while"
g++ -g -ftime-report -std=c++17 -pthread *.cpp *.h -o main.exe

echo "File build complete, running HFT-market-sim"
valgrind --leak-check=full ./main.exe