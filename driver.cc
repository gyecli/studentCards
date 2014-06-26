//
//  driver.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/24/2013.
//
//

#include <uC++.h>
#include <iostream>
#include <cstdlib>          // access: rand, srand
#include <unistd.h>         // access: getpid
#include <cstring>
#include <string>
#include "config.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcardOffice.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
#include "student.h"

using namespace std;

MPRNG mprng;                    // Globally declare mprng
unsigned int numFlavour = 4;    // Number of flavours: Blues Black-Cherry,Classical Cream-Soda,Rock Root-beer,Jazz Lime
void processConfigFile( const char *configFile, ConfigParms &cparms );

//======================================================================================================
// Show the correct usage
void usage()
{
    // Usage: ./soda [ config-file [ random-seed (> 0) ] ]
    cerr << "Usage: ./soda [ config-file [ random-seed (> 0) ] ]" << endl;
    return;
}

//======================================================================================================
// Main
void uMain::main()
{
    /* set default values for seed and config file */
    int seed = getpid();
    ConfigParms cparms;                             // Configuration arguments
    const char * defaultFile = "soda.config";       // Default configuration file is soda.config
    char *configFile;
    
    /* read command-line arguments */
    switch (argc) {
        case 1:
            processConfigFile(defaultFile, cparms);
            srand(seed);
            break;
        case 2:
            configFile = argv[1];
            processConfigFile(configFile, cparms);
            srand(seed);
            break;
        case 3:
            configFile = argv[1];
            processConfigFile(configFile, cparms);
            seed = atoi(argv[2]);
            if (seed <= 0)
            {
                usage();
                return;
            }
            mprng.seed(seed);
            break;
        default:
            usage();
            return;
    }
    
    /* Load all the config arguments from cparms */
    unsigned int sodaCost = cparms.sodaCost;                        // MSRP per bottle
    unsigned int numStudents = cparms.numStudents;                  // number of students to create
    unsigned int maxPurchases = cparms.maxPurchases;                // maximum number of bottles a student purchases
    unsigned int numVendingMachines = cparms.numVendingMachines;    // number of vending machines
    unsigned int maxStockPerFlavour = cparms.maxStockPerFlavour;    // maximum number of bottles of each flavour stocked
    unsigned int maxShippedPerFlavour = cparms.maxShippedPerFlavour;// number of bottles of each flavour in a shipment
    unsigned int timeBetweenShipments = cparms.timeBetweenShipments;// length of time between shipment pickup
    unsigned int parentalDelay = cparms.parentalDelay;              // length of time between cash deposits
    unsigned int numCouriers = cparms.numCouriers;                  // number of couriers in the pool
    
    /* initializaton for the printer, the bank, the parent, the watcardoffice, the nameserver, vending machines,
    the bottling plant and students */
    Printer printer(numStudents, numVendingMachines, numCouriers);                          // Create Printer
    Bank bank(numStudents);                                                                 // Create Bank
    Parent *parent = new Parent( printer, bank, numStudents, parentalDelay );               // Create Parent
    WATCardOffice *cardOffice = new WATCardOffice( printer, bank, numCouriers );            // Create WATCardOffice
    NameServer * nameServer = new NameServer( printer, numVendingMachines, numStudents );   // Create NameServer

    /* creating vending machines */
    VendingMachine *vendingMachines[numVendingMachines];
    for (unsigned int k = 0; k < numVendingMachines; k++)
    {
        vendingMachines[k] = new VendingMachine(printer, *nameServer,k,sodaCost,maxStockPerFlavour);
    }
    
    /* creating the bottling plant */
    BottlingPlant *plant = new BottlingPlant(printer, *nameServer, numVendingMachines, maxShippedPerFlavour,
                                             maxStockPerFlavour, timeBetweenShipments );
    
    /* creating students */
    Student *students[numStudents];
    for (unsigned int k = 0; k < numStudents; k++)
    {
        students[k] = new Student( printer,*nameServer,*cardOffice,k, maxPurchases);
    }

    /* Start doing deallocation in reverse order */
    for (int k = 0; k < (int)numStudents; k++)
    {
        delete students[k];
    }

    delete plant;
    for (int k = 0; k < (int)numVendingMachines; k++)
    {
        delete vendingMachines[k];
    }
    delete nameServer;
    delete cardOffice;
    delete parent;
}
//valgrind --tool=memcheck --leak-check=full ./soda
