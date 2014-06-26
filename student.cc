//
//  student.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include <uFuture.h>
#include "MPRNG.h"
#include "student.h"
#include "printer.h"
#include "watcardOffice.h"
#include "watcard.h"
#include "nameServer.h"
#include "vendingMachine.h"

extern MPRNG mprng;                     // Defined in driver.cc
extern unsigned int numFlavour;         // numFlavour == 4;

//======================================================================================================
// Task main
void Student::main()
{
    numToBuy = mprng(1, maxPurchases);                          // Number of bottles to buy
    favourite = mprng(numFlavour-1);                            // Favourite flavour to buy [0, 3]
    prt.print(Printer::Student, id,'S', favourite, numToBuy);   // Start msg
    
    fCard = cardOffice.create(id, 5);                           // Create a future WATCard pointer
    myMachine = nameServer.getMachine(id);                      // Get a pointer of location of the vending machine
                                                                //   via NameServer
    prt.print(Printer::Student, id, 'V', myMachine->getId());   // Msg of selecting vending machine
    
    unsigned int numLeft = numToBuy;                            // Number of bottles the student still need to buy
    for ( ;numLeft > 0; )
    {
        yield(mprng(1, 10));                                    // Yield before a buy
        VendingMachine::Status status;                          // Status of buy
        while (true)
        {
            try {
                // Attempt to buy a soda
                // The student blocks until fCard is delivered
                myCard = &(*fCard());          // Synchronize on retrieve result
        
                status = myMachine->buy(VendingMachine::Flavours(favourite), *myCard);
                goto L;
                //break;
            }
            catch (WATCardOffice::Lost)                 // The WATCard is lost by courier
            {
                prt.print(Printer::Student,id,'L');     // Msg of WATCard lost
                fCard.cancel();                         // Make fCard empty so that it can be reused
                fCard = cardOffice.create(id, 5);       // "Create" a new WATCard
            }
        }
    L:
        if (status == VendingMachine::STOCK)        // The vending machine is out of the student's favourite flavour
        {
            myMachine = nameServer.getMachine(id);  // Obtain a new vending machine from the name server
        }
        else if (status == VendingMachine::FUNDS)   // Insufficient funds
        {
            fCard = cardOffice.transfer(id, 5+myMachine->cost(), myCard);   // Transfer soda-cost + $5 to the card
            
        }
        else                                        // Successfully bought the soda
        {
            prt.print(Printer::Student,id,'B',myCard->getBalance());        // Msg of bought a soda
            numLeft -= 1;
        }
    }//for
    prt.print(Printer::Student,id,'F');             // Finished msg
}

//======================================================================================================
// Contructor for student
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
                 unsigned int maxPurchases ) : prt(prt), nameServer(nameServer), cardOffice(cardOffice),
                 id(id), maxPurchases(maxPurchases) {}

//======================================================================================================
// Destructor for Student
Student::~Student()
{
    delete fCard;       // Delete funture
}
