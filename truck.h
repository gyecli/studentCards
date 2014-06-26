//
//  truck.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____truck__
#define ____truck__

#include <uC++.h>
#include <iostream>

_Monitor Printer;           // Forward declarations
_Task NameServer;
_Task BottlingPlant;
_Task VendingMachine;   

_Task Truck {
    Printer &prt;                           // Printer
    NameServer &nameServer;                 // NameServer
    BottlingPlant &plant;                   // BottlingPlant
    unsigned int numVendingMachines;        // Number of vending machines
    unsigned int maxStockPerFlavour;        // Maximum number of bottles of each flavour in a vending machine
    unsigned int *cargo;                    // Cargo: array of # of bottles for each flavour
    VendingMachine **machineList;           // List of vending machines from NameServer
    
    void main();                            // Task main
    
public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
          unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};

#endif /* defined(____truck__) */
