//
//  nameServer.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include <list>
#include "nameServer.h"
#include "printer.h"
#include "vendingMachine.h"

//======================================================================================================
// NameServer is an adminstrator task used to manage the vending machine names
//======================================================================================================
// Task main for NameServer
void NameServer::main()
{
    prt.print(Printer::NameServer, 'S');                        // Start msg
    for(;;)
    {
        _Accept(~NameServer)                                    // when the destructor is called
        {
            break;
        }
        or _Accept(VMregister)                                  // a vending machine is registering
        {
            numRegister += 1;                                   // Increment num of machines that have registered
            prt.print(Printer::NameServer, 'R', mid);           // Msg of veding machine registering
        }
        or _When(numRegister == numVendingMachines) _Accept(getMachine)     // Wait until all machines registered
        {
            prt.print(Printer::NameServer, 'N', sid, mid);      // Msg of student requesting vending machine
        }
        or _When(numRegister == numVendingMachines) _Accept(getMachineList) // Wait until all machines registered
        {
            
        }
    }
    prt.print(Printer::NameServer, 'F');                        // Finished msg
}

//======================================================================================================
// Constructor for NameServer
NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) :
    prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents)
{
    numRegister = 0;
    machineArray = new VendingMachine*[numVendingMachines]; // Array of vending machine pointers
    assignedArray = new unsigned int[numStudents];          // Array about which machine each student is assigned to
    for (unsigned int k = 0; k < numStudents; k++)
    {
        assignedArray[k] = k % numVendingMachines;          // Distribute students evenly across the vending machines
    }
}

//======================================================================================================
// Destructor for NameServer
NameServer::~NameServer()
{
    delete[] machineArray;
    delete[] assignedArray;
}

//======================================================================================================
// Vending machines call VMregister() to register themselves so students can subsequently locate them
void NameServer::VMregister( VendingMachine *vendingmachine )
{
    mid = vendingmachine->getId();  // the machine that is registering
    machineArray[mid] = vendingmachine;          // Put the pointer of the vending machine into the array
    return;
}

//======================================================================================================
// A student calls getMachine() find a vending machine
VendingMachine* NameServer::getMachine( unsigned int id )
{
    sid = id;                                       // the id of the student who wants to get a machine
    mid = assignedArray[id];                        // Get the vending machine id that assigned to the student
    VendingMachine* targetM = machineArray[mid];    // Get the vending machine that will be returned
    assignedArray[id] = (assignedArray[id] + 1) % numVendingMachines;   // Modulo increment so that each student will
                                                                        // eventually visit every machine
    return targetM;
}

//======================================================================================================
// The truck calls getMachineList() to obtain an array of pointers to vending machines so it can visit each
//   machine to deliver new soda
VendingMachine ** NameServer::getMachineList()
{
    return machineArray;
}
//======================================================================================================
