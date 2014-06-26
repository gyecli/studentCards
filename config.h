//
//  config.h
//
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

//#ifndef ____configFile__
//#define ____configFile__

#include <iostream>
#include <fstream>

struct ConfigParms {
    unsigned int sodaCost;                 // MSRP per bottle
    unsigned int numStudents;              // number of students to create
    unsigned int maxPurchases;             // maximum number of bottles a student purchases
    unsigned int numVendingMachines;       // number of vending machines
    unsigned int maxStockPerFlavour;       // maximum number of bottles of each flavour stocked
    unsigned int maxShippedPerFlavour;     // number of bottles of each flavour in a shipment
    unsigned int timeBetweenShipments;     // length of time between shipment pickup
    unsigned int parentalDelay;            // length of time between cash deposits
    unsigned int numCouriers;              // number of couriers in the pool
};

void processConfigFile( const char *configFile, ConfigParms &cparms );

//#endif /* defined(____configFile__) */