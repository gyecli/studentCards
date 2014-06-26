//
//  parent.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include "parent.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"

extern MPRNG mprng;

//======================================================================================================
// Task main
void Parent::main()
{
    prt.print(Printer::Parent, 'S');                // Starting msg
    for (;;)
    {   // Must check for a call to its destructor to know when to terminate
        // Since it must not block on this call, so _Else is used, and busy waiting exists
        _Accept (~Parent)
        {
            break;
        }
        _Else
        {
            yield(parentalDelay);
            unsigned int sid = mprng(numStudents-1);    // Choose a random student
            unsigned int cash = mprng(1, 3);            // Give random amount of money [$1, $3]
            bank.deposit(sid, cash);                    // Deposit the money for that particular student
            prt.print(Printer::Parent,'D',sid,cash);    // Msg of deposit gift to the student
        }
    }
}

//======================================================================================================
// Constructor for Parent
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) :
    prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}

//======================================================================================================
// Destructor for Parent
Parent::~Parent(){
    prt.print(Printer::Parent,'F');
}