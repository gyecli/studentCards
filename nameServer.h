//
//  nameServer.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____nameServer__
#define ____nameServer__

#include <uC++.h>

_Monitor Printer;                       // Forward declarations
_Task VendingMachine;

_Task NameServer {
    Printer &prt;                       // Printer
    unsigned int numVendingMachines;    // Number of vending machines
    unsigned int numStudents;           // Number of students

    unsigned int numRegister;           // Number of vending machines that have registered
    unsigned int mid;                   // ID of the vending machine 
    unsigned int sid;                   // the student who is asking for soda
    unsigned int *assignedArray;        // Array with each element be a vending machine assigned to it
    VendingMachine **machineArray;      // Array of the pointers for all vending machines
    
    void main();
public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif /* defined(____nameServer__) */
