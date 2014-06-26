//
//  printer.cc
//
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include "printer.h"

//======================================================================================================
// Constructor for Printer
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers)
{
    // Parent + WATOff + NameServer + Truck + Plant + numStudents + numVendingMachine + numCouriers
    numFacilities = 5;
    totalMembers = numFacilities + numStudents + numVendingMachines + numCouriers;
    
    empty = true;                                   // The buffer array is initially empty
    occupied = new bool[totalMembers];              // Declare and initialize the occupied array
    for (int k = 0; k < (int)totalMembers; k++)
    {
        occupied[k] = false;
    }
    printArray = new printInfo[totalMembers];       // Declare an buffer array
    for(int k = 0; k < (int)totalMembers; k++)
    {
        printArray[k].state = ' ';
        printArray[k].value1 = -1;
        printArray[k].value2 = -1;
    }
    
    // Print the start up message
    std::cout << "Parent \tWATOff \tNames  \tTruck  \tPlant  \t";
    for (int k = 0; k < (int)numStudents; k++)
    {
        std::cout << "Stud" << k << "  \t";
    }
    for (int k = 0; k < (int)numVendingMachines; k++)
    {
        std::cout << "Mach" << k << "  \t";
    }
    for (int k = 0; k < (int)numCouriers; k++)
    {
        std::cout << "Cour" << k << "  \t";
    }
    std::cout << "\n";
    
    for (int k = 0; k < (int)totalMembers; k++)
    {
        std::cout << "*******\t";
    }
    std::cout << "\n";
}


//======================================================================================================
// Destructor for Printer
Printer::~Printer()
{
    std::cout << "***********************" << std::endl;
    
    delete[] occupied;
    delete[] printArray;
}

//======================================================================================================
// First version
void Printer::print( Kind kind, char state )
{
    print(kind, totalMembers, state, -1, -1);              // Transfer to third version
}

//======================================================================================================
// Second version
void Printer::print( Kind kind, char state, int value1 )
{
    print(kind, totalMembers, state, value1, -1);         // Transfer to third version
}

//======================================================================================================
// Third version
void Printer::print( Kind kind, char state, int value1, int value2 )
{
    print(kind, totalMembers, state, value1, value2);
}

//======================================================================================================
// Fourth version
void Printer::print( Kind kind, unsigned int lid, char state )
{
    print(kind, lid, state, -1, -1);
}

//======================================================================================================
// Fifth version
void Printer::print( Kind kind, unsigned int lid, char state, int value1 )
{
    print(kind, lid, state, value1, -1);
}

//======================================================================================================
// Sixth version
// If kind == one of Student, Vending and Courier, we calculate the index of the printing array using lid
// Otherwise, lid is the index of the printing array
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 )
{
    // Calculate the index
    int index;
    if (kind == Student)
    {
        index = numFacilities + lid;
    }
    else if (kind == Vending)
    {
        index = numFacilities + numStudents + lid;
    }
    else if (kind == Courier)
    {
        index = numFacilities + numStudents + numVendingMachines + lid;
    }
    else
    {
        index = (int)kind;              // Otherwise, index is the order that kind is listed in enum
    }
    
   
    // ********************************* //
    if ((state == 'F' && !empty) || occupied[index])                // TODO:
    {
        for (int k = 0; k < (int)totalMembers; k++)                 // Flush the whole buffer array
        {
            if (occupied[k])    // kth element has sth, so print it
            {
                std::cout << printArray[k].state;                   // Print state
                
                int val1 = printArray[k].value1;
                int val2 = printArray[k].value2;
                if (val2 != -1)                                     // Two values are specified
                {
                    std::cout << val1 << "," << val2 << "   \t";
                }
                else if (val1 != -1)                                // Value 1 is specified, but value2
                {
                    std::cout << val1 << "     \t";
                }
                else                                                // No values are specified
                {
                    std::cout << "      \t";
                }
            }//if
            else
            {
                std::cout << "       \t";
            }//else
            
            occupied[k] = false;        // kth element is now emtpy
            printArray[k].state = ' ';
            printArray[k].value1 = -1;
            printArray[k].value2 = -1;
        }//for
        empty = true;                   // Buffer array is not empty
        std::cout << "\n";
    }//if
    
    if (state == 'F')
    {
        // (After flushing,) print 'F'
        for (int k = 0; k < (int)totalMembers; k++)
        {
            if ( index == k)
            {
                std::cout << "F      \t";
            }
            else {
                std::cout << "...    \t";
            }
        }//for
        std::cout << "\n" << std::endl;
    }//if
    else if (!occupied[index])
    {
        occupied[index] = true;                     // The slot is nonempty now
        printArray[index].state = state;
        printArray[index].value1 = value1;
        printArray[index].value2 = value2;
        
        empty = false;                              // Buffer array now is non-empty
    }
}
