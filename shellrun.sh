#!/bin/bash

# run in ubuntu terminal

rm -f simulation.exe

g++ -std=c++17 -pthread *.cpp *.h -o simulation.exe

./simulation.exe
