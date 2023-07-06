# HFT-market-sim

## Tools used in the creation of this project: 

> Standard Libraries from chronos, threads, mutex, atomic, etc.

> leaks & Valgrind memory management, GDB/clang++ debugger (this project started off on Windows, but was completed on macOS)

## Summary

This is my high-frequency trading market simulator, where through concurrent and parallel programming (through the use of multithreading) I have 25 traders all making and handling orders with their portfolio of Nasdaq100 stocks. All 25 traders are operating simultaneously across the computer's cores, concurrently with market fluctuations, generated marketevents, and orderbook executions. 

Each trader thread has decisions decided by a number generator about buying/selling stocks, or cancelling an active order. The quantities, as well as specific stock, is also decided by a generator. The process of ordering is handled through the orderbook, and the portfolios of each individual trader is dynamically and independantly updated at the fulfillment of their orders. 

The fluctuation thread generates numbers that when passed to each stock, will randomly fluctuate the stocks independant of one another, so the price of each stock is changing. Note that this will not affect placed orders, as those set prices are final. 

The market event thread generates market events (currently interest rate changes, public opinion changes, global economic changes, recessions, times of prosperity, an umbrella 'other government policy', or nothing). Each event (excluding nothing, of course) affects stock prices through multiplying their price (affected via the fluctuations thread) by a combined factor of events simply called 'factors'. 

The orderbook execution thread does as the name suggests, executes the placed orders in the orderbook and adjust respective trader's portfolios and cash respectively. 

Finally, the main thread sets up the simulation via reading in stock and trader data, setting up marketevent odds, and starting the threads. At the conclusion of 5 minutes (arbitrary time, can be modified in the _Market::run()_ method) the simulation will stop, the threads will join, and everything will be reset. Future works include providing interesting statistics.

All files are necessary to run this program. The program should be run through the terminal ('./shellrun.sh'), making sure to have executable permissions('chmod +x shellrun.sh'). The license can be found [here](LICENSE). All bugs and improvement suggestions should be emailed to _alexander.k.nichols@gmail.com_. 