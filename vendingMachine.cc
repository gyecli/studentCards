//
//  vendingMachine.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include "vendingMachine.h"
#include "printer.h"
#include "nameServer.h"
#include "watcard.h"

extern unsigned int numFlavour;          // Defined in driver.cc. numFlavour == 4;

//======================================================================================================
// Constructor for VendingMachine
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                unsigned int maxStockPerFlavour ) : prt(prt), nameServer(nameServer), id(id),sodaCost(sodaCost),
                maxStockPerFlavour(maxStockPerFlavour)
{
    index = 0;
    currentStock = new unsigned int[numFlavour];    // Array of stocks
    for (int k = 0; k < (int)numFlavour; k++)       // Initialize the stock array to be all zero
    {
        currentStock[k] = 0;
    }
    reloading = false;                              // The vending machine is not being stocked currently
}

//======================================================================================================
// Destructor for VendingMachine
VendingMachine::~VendingMachine()
{
    delete[] currentStock;
}

//======================================================================================================
// Task main
void VendingMachine::main()
{
    prt.print(Printer::Vending, id, 'S', (int)sodaCost);// Starting message with cost of each soda
    nameServer.VMregister(this);                        // Register itself with the name server
    
    for(;;)
    {
        _Accept(~VendingMachine)                        // Destructor was called
        {
            break;
        }
        or _Accept(inventory)
        {
            reloading = true;                           // Reloading
            prt.print(Printer::Vending, id, 'r');       // Start reloading msg
        }
        or _Accept(restocked)
        {
            reloading = false;                          // Reloading is finished
            prt.print(Printer::Vending, id, 'R');       // Msg of complete reloading by truck
        }
        or _When(!reloading) _Accept(buy) {}            // When reloading, the vending machine cannot accept buy calls
    }
    prt.print(Printer::Vending, id, 'F');               // Finished msg
}

//======================================================================================================
// Student calls buy() to obatain one of their favourite sodas
// If the specified soda is unavailable, return STOCK
// If the student has insufficient funds to purchase the soda,return FUNDS
// Otherwise, the student’s WATCard is debited by the cost of a soda and returns BUY.
VendingMachine::Status VendingMachine::buy( Flavours flavour, WATCard &card )
{
    index = (int)flavour;               // Get the index of the flavour specified in enum
                                        //   BlackCherry(0), CreamSoda(1), RootBeer(2), JazzLime(3)
    if (currentStock[index] <= 0) {
        return STOCK;                   // Specified soda is sold out
    }
    if (card.getBalance() < sodaCost)
    {
        return FUNDS;                   // Insufficient funds in watcard
    }
    card.withdraw(sodaCost);            // Withdraw cost from the card
    currentStock[index] -= 1;           // Decrement of stock of specified flavour by 1
    prt.print(Printer::Vending, id, 'B', index, (int)currentStock[index]); // Msg of student bought a soda
    return BUY;                         // Purchase successfully
}

//======================================================================================================
// The truck calls inventory() to return a pointer to an array containing the amount of each kind of soda
//   currently in the vending machine.
unsigned int* VendingMachine::inventory()              
{
    return currentStock;                // Return the pointer to the stock array
}

//======================================================================================================
// After transferring new soda into the machine by directly modifying the array passed from inventory,
//   the truck calls restocked to indicate the operation is complete
void VendingMachine::restocked()
{
    return;
}

//======================================================================================================
// Return cost of purchasing a soda for this machine
// (This is a _Nomutex member)
unsigned int VendingMachine::cost()
{
    return sodaCost;
}

//======================================================================================================
// Return the ID number of the vending machine
// (This is a _Nomutex member)
unsigned int VendingMachine::getId()
{
    return id;
}
