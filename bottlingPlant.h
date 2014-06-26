//
//  bottlingPlant.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____bottlingPlant__
#define ____bottlingPlant__

#include <uC++.h>
#include <iostream>

_Monitor Printer;                       // Forward declaration
_Task NameServer;
_Task Truck;


_Task BottlingPlant {
    
    Printer &prt;                       // Printer
    NameServer &nameServer;             // NameServer -- An administrator task used to manage the vending-machine names
    Truck *truck;                       // Truck that deliverys sodas to vendingmahcine
    
    unsigned int numVendingMachines;    // The number of vending machines
    unsigned int maxShippedPerFlavour;  // The maximum number of bottles of each flavour generated during a production
    unsigned int maxStockPerFlavour;    // The maximum number of bottles of each flavour in a vending machine
    unsigned int timeBetweenShipments;  // The length of time between shipment pickups by the truck
    unsigned int totalAmount;           // Total amount of soda produced at each run
    unsigned int *production;           // Array of each flavour produced at each production run
    bool closing;                       // Indicate the plant is closing down
    
    void main();                        // Task main
    
public:
    BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                  unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                  unsigned int timeBetweenShipments );
    ~BottlingPlant();
    
    bool getShipment( unsigned int cargo[] );   // truck calls to get sodas into its cargo
};

#endif /* defined(____bottlingPlant__) */
