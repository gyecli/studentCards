//
//  bottlingPlant.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include "MPRNG.h"
#include "bottlingPlant.h"
#include "printer.h"
#include "nameServer.h"
#include "truck.h"

extern MPRNG mprng;                     // Defined in driver.cc
extern unsigned int numFlavour;         // Defined in driver.cc. numFlavour == 4;

//======================================================================================================
// Task main
void BottlingPlant::main()
{
    prt.print(Printer::BottlingPlant, 'S');                                             // Start msg
    
    truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);  // Create a truck
    
    for (;;)
    {
        yield(timeBetweenShipments);                                    // To simulate a production run of soda
        
        totalAmount = 0;                                                // Total amount of soda produced
        
        for (int k = 0; k < (int)numFlavour; k++)                       // Produce a new quantity of sodas for each flavour
        {
            unsigned int numPerFlavour = mprng(maxShippedPerFlavour);   // Produce a random new quatities of soda for flavour k
            production[k] = numPerFlavour;                              // Load into production array
            totalAmount += numPerFlavour;                               // Increment total amount
        }
        prt.print(Printer::BottlingPlant, 'G', (int)totalAmount);       // Generating msg
        
        _Accept(~BottlingPlant)
        {
            closing = true;             // The plant is closing down and need to inform the truck

            _Accept(getShipment) {      // need to wait the truck for coming back and knowing that the plant is closed

            }
            delete truck;               // Delete the truck
            break;                      // leave the loop
        }
        or _Accept(getShipment)
        {
            prt.print(Printer::BottlingPlant, 'P');     // Shipment picked up msg
        }
    }
    prt.print(Printer::BottlingPlant, 'F');             // Finished msg
}

//======================================================================================================
// Contructor for BottlingPlant
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments ) :
    prt(prt),nameServer(nameServer),numVendingMachines(numVendingMachines),maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour),timeBetweenShipments(timeBetweenShipments)
{
    closing = false;                            // Closing flag
    production = new unsigned int[numFlavour];  // Initialize first shipment
    for (int k = 0; k < (int)numFlavour; k++)
    {
        production[k] = 0;
    }
}

//======================================================================================================
// Destructor for BottlingPlant
BottlingPlant::~BottlingPlant()
{
    delete[] production;
}

//======================================================================================================
// The truck calls getShipment() to obtain a shipment from the plant(i.e., the production run),
//   and the shipment is copied into the cargo array passed by the truck.
// getShipment() returns TRUE if the bottling plant is closing down and cargo is not changed,
//   and FALSE otherwise with the shipment copied into the cargo array passed by the truck.
bool BottlingPlant::getShipment( unsigned int cargo[] )
{
    if (closing)                                // The plant is closing down, and cargo is not changed
    {
        return true;
    }
    
    /* otherwise, put sodas on the truck */
    int k;
    for (k = 0; k < (int)numFlavour; k++)       // Load the cargo
    {
        cargo[k] = production[k];
    }
    cargo[k] = totalAmount;                     // Store total amount of cargo into the last element of the array
    
    return false;
}
