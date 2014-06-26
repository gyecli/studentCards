//
//  parent.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____parent__
#define ____parent__

#include <uC++.h>

_Monitor Printer;
_Monitor Bank;

_Task Parent {
    Printer &prt;               // Printer
    Bank &bank;                 // Bank
    unsigned int numStudents;   // Number of students
    unsigned int parentalDelay; // Length of time between cash deposits
    
    void main();
public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};

#endif /* defined(____parent__) */
