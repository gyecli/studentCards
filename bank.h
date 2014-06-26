//
//  bank.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____bank__
#define ____bank__

#include <uC++.h>

_Monitor Bank {
    unsigned int numStudents;   // Number of students
    unsigned int *accountInfo;  // Array that stores the account balances of all students
    uCondition* enough;         // Condition variable which couriers will sleep on when there is no enough money to withdraw
    
public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif /* defined(____bank__) */
