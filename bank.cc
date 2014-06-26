//
//  bank.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include "bank.h"


//======================================================================================================
// Constructor for Bank
Bank::Bank( unsigned int numStudents )
{
    accountInfo = new unsigned int[numStudents];    // Array storing the account balances of all students
    for (int k = 0; k < (int)numStudents; k++)      // All accounts start with $0 balance
    {
        accountInfo[k] = 0;
    }
    enough = new uCondition[numStudents];
}

//======================================================================================================
// Destructor for Bank
Bank::~Bank()
{
    delete[] accountInfo;
    delete[] enough;
}

//======================================================================================================
// The parent calls this to deposit money for the specific student
void Bank::deposit( unsigned int id, unsigned int amount )
{
    accountInfo[id] += amount;              // Deposit the amount
    enough[id].signal();                    // Signal the courier waiting for the deposit (if there is one)
}

//======================================================================================================
// A courier calls this to witdraw and transfer money on behalf of the WATCard officefor a specific student
// The courier WAIT until enough money has been deposited, which may require multiple deposits
void Bank::withdraw( unsigned int id, unsigned int amount )
{
    for(;;)
    {
        if (accountInfo[id] >= amount)      // There is enough amount
        {
            accountInfo[id] -= amount;      // Withdraw
            break;
        }
        enough[id].wait();                  // Otherwise, wait until there is enough
        
    }//for
}
