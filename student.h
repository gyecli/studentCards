//
//  student.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____student__
#define ____student__

#include <uC++.h>
#include "watcard.h"

_Monitor Printer;       // Forward declaration
_Task NameServer;
_Task WATCardOffice;
_Task VendingMachine;

_Task Student {
    Printer &prt;                   // Pritner
    NameServer &nameServer;         // NamerServer
    WATCardOffice &cardOffice;      // WATCardOffice
    unsigned int id;                // Student id
    unsigned int maxPurchases;      // Max number of purchase each student can buy
    
    unsigned int numToBuy;          // Number of bottles to purchase;
    unsigned int favourite;         // Favourite flavour
    WATCard *myCard;                // WATCard
    WATCard::FWATCard fCard;        // Future WATCard
    VendingMachine *myMachine;      // The vending machine that the student goes for
    
    void main();
public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
            unsigned int maxPurchases );
    ~Student();
};

#endif /* defined(____student__) */
