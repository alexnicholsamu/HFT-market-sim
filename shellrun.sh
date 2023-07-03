#!/bin/bash

echo "Deleting old main.exe, if it exists"
rm -f main.exe

echo "Starting build, may take a little while"
g++ -std=c++17 -pthread *.cpp *.h -o main.exe 2>&1

if [ $? -eq 0 ]; then
    echo "File build complete, running HFT-market-sim"
    ./main.exe
else
    echo "Compilation failed."
fi