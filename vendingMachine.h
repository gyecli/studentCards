//
//  vendingMachine.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____vendingMachine__
#define ____vendingMachine__

#include <uC++.h>
#include <iostream>

_Monitor Printer;                       // Forward delcarations
_Task NameServer;
class WATCard;

_Task VendingMachine {
    Printer &prt;                       // Printer
    NameServer &nameServer;             // NameServer
    unsigned int id;                    // ID for this vending machine
    unsigned int sodaCost;              // Cost for each soda
    unsigned int maxStockPerFlavour;    // Maximum number of bottles of each flavour stocked
    
    unsigned int *currentStock;         // Array of stocks for each flavour
    int index;                          // Index of the flavour in a vending machine
    WATCard *watCard;                      // WATCard;
    bool reloading;                     // Indicate that the vending machine is being stocked and won't accpet any buys
    
    void main();
    
public:
    enum Flavours { BlackCherry, CreamSoda, RootBeer, JazzLime };   // flavours of soda (YOU DEFINE)
    enum Status { BUY, STOCK, FUNDS };  // purchase status: successful buy, out of stock, insufficient funds
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                   unsigned int maxStockPerFlavour );
    ~VendingMachine();
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif /* defined(____vendingMachine__) */
