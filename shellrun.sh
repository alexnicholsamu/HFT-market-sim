#!/bin/bash

echo "Deleting old main, if it exists"
rm -f main.exe

echo "Starting build, may take a little while"
clang++ -g -std=c++17 -pthread *.cpp -o main.exe

echo "File build complete, running HFT-market-sim"
./main.exe

echo "Checking for memory leaks"
leaks -atExit -- ./main.exe
