#!/bin/bash

# run in ubuntu terminal

echo "Deleting old main.exe, if it exists"

rm -f main.exe

echo "Starting build, may take a little while"

g++ -std=c++17 -pthread *.cpp *.h -o main.exe

echo "File build complete, running main.exe"

./main.exe