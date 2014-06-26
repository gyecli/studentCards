//
//  truck.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include "MPRNG.h"
#include "truck.h"
#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "vendingMachine.h"

extern MPRNG mprng;             // Defined in driver.cc
extern int numFlavour;          // numFlavour == 4, defined in driver.cc

//======================================================================================================
void Truck::main()
{
    prt.print(Printer::Truck, 'S');                 // Starting msg
    machineList = nameServer.getMachineList();      // Get list of locations of each vending machine
    
    for (;;)
    {
        yield(mprng(1,10));                         // Get a coffee from Tom Horton
        
        // Throw old soda away, because it is past its due date
        for (int k = 0; k < (int)(numFlavour+1); k++)
        {
            cargo[k] = 0;
        }
        
        // Obtain a new shipment
        
        if (plant.getShipment(cargo))
        {
            break;                                                      // The bottling plant is closing down
        }

        prt.print(Printer::Truck, 'P', cargo[numFlavour]);              // Msg of picked up shipment
                                                                        // Last element in cargo array stores total num
        // Visit every vending machine in the list OR until cargo is empty
        for (unsigned int k = 0; k < numVendingMachines && cargo[numFlavour] > 0; k++)
        {
            prt.print(Printer::Truck, 'd', k, cargo[numFlavour]);       // Msg of begin delivery to a vending machine
            
            VendingMachine *nextMachine = machineList[k];               // Get next vending machine
            unsigned int *currentInventory = nextMachine->inventory();  // Get current stock of the vending machine
                                                                        //   of soad currently in the vending machine
            unsigned int notReplenished = 0;                            // Total num of bottles not replenished
            
            // Loop thru each flavour in this vending machine
            for (int i = 0; i < (int)numFlavour; i++)
            {
                unsigned int needs = maxStockPerFlavour - currentInventory[i];  // The amount that it needs to fill
                                                                                //   each flavour
                if (needs > cargo[i])                           // Current flavour in cargo is less than what's needed
                {
                    cargo[numFlavour] -= cargo[i];              // Decrement total amount in cargo
                    notReplenished += (needs - cargo[i]);
                    currentInventory[i] += cargo[i];            // Restock what's left for this flavour in the cargo
                    cargo[i] = 0;                               // Current flavour in cargo is empty
                }
                else                                            // Current flavour in cargo is enough for restocking
                {
                    cargo[numFlavour] -= needs;                 // Decrement total amount in cargo
                    currentInventory[i] = maxStockPerFlavour;   // Current flavour in the vending machine is now full
                    cargo[i] -= needs;                          // Decrement cargo
                }
            }//for -- loop thru each flavour in this vending machine
            
            nextMachine->restocked();                           // To indicate stocking is finished for this machine
            
            if (notReplenished > 0)
            {
                prt.print(Printer::Truck, 'U', k, notReplenished);  // Msg of unsuccessfully filled vending machien
            }
            prt.print(Printer::Truck, 'D', k, cargo[numFlavour]);   //Msg of end delivery to the vending machine
        }//for
    }//for - main loop
    
    // TODO: The bottling plant is closing down, so truck terminates
    prt.print(Printer::Truck, 'F');
}
//======================================================================================================
// Constructor for Truck
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
              unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
    prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour)
{
    cargo = new unsigned int[numFlavour+1];          // Initialize the cargo for each flavour
    for (int k = 0; k < (int)(numFlavour+1); k++)    // The last element of cargo indicate the total amount
    {
        cargo[k] = 0;
    }
}

//======================================================================================================
// Detructor for Truck
Truck::~Truck()
{
    delete[] cargo;
}
