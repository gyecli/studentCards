//
//  printer.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____printer__
#define ____printer__

#include <uC++.h>
#include <iostream>

_Monitor Printer {
    typedef struct
    {
        char state;                     // State to be printed
        int value1;                     // first value to be printed, if no specipied value, should be -1
        int value2;                     // second value to be printed, if no specified value, should be -1
    } printInfo;
    
    unsigned int numStudents;           // Number of students
    unsigned int numVendingMachines;    // Number of vending machines
    unsigned int numCouriers;           // Number of couriers
    
    unsigned int totalMembers;          // Total number of all involved members
    int numFacilities;                  // Total number of facilities: Parent(s),WATOff(s),Server(s),Truck(s),Plant(s)
                                        //   not include students, vending machines or courier
    bool *occupied;                     // Array that indicate if each element in the printing buffer already has sth
    printInfo *printArray;              // Array that stores all the printing info
    bool empty;                         // Indicate the whole buffer is emtpy
    int mode;                           // Used to specify which print() function is called
    
public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};

#endif /* defined(____printer__) */
