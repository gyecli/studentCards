//
//  watcardOffice.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include <uC++.h>
#include <uFuture.h>
#include "watcardOffice.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"
#include "watcard.h"

extern MPRNG mprng;
//======================================================================================================
// This is Courier
//======================================================================================================
// Task main for Courier
void WATCardOffice::Courier::main()
{
    try
    {
        prt_.print(Printer::Courier,id_,'S');       // Courier start message
        for (;;)
        {
            Job *myJob = office->requestWork();     // Request for a job. BLOCKs if no ready requests
            
            unsigned int studentID = myJob->sid;    // Student id
            unsigned int amt = myJob->amount;       // Amount of money requested
            WATCard *card = myJob->watcard;         // Target WATCard

            prt_.print(Printer::Courier, id_, 't', studentID,amt);   // Msg of start funds transfer
            bank_.withdraw(studentID, amt);     // Withdraw money from bank. BLOCKs if insufficient amount in the bank
            card->deposit(amt);                 // Deposit the amount into WATCard
            prt_.print(Printer::Courier, id_, 'T', studentID, amt);  // Msg of complete funds transfer
            
            if (mprng(5) == 0)                  // 1 in 6 chance to lose the WATCard
            {
                // exception() copies exception into future, and exception is thrown at waiting clients
                delete myJob->watcard;
                (myJob->result).exception(new WATCardOffice::Lost);// Raise Lost exception if card lost TODO: corrent way?
            }
            else
            {
                (myJob->result).delivery(card); // Deliver the result
            }
        }
    }
    catch (Stop) {}     // Catch Stop exception
}

//======================================================================================================
// Constructor for Courier
WATCardOffice::Courier::Courier( Printer &prt_, Bank &bank_, WATCardOffice * office, unsigned int id_ ) :
    prt_(prt_),bank_(bank_),office(office), id_(id_) {}

//======================================================================================================
// Destructor for Courier
WATCardOffice::Courier::~Courier()
{
    prt_.print(Printer::Courier, id_, 'F');   // Finished msg
}




//======================================================================================================
//======================================================================================================
//======================================================================================================
// Task main for WATCardOffice
void WATCardOffice::main()
{
    prt.print(Printer::WATCardOffice,'S');          // Starting message
    for (;;)
    {
        _Accept(~WATCardOffice)
        {
            done = true;                            // Let all couriers knows that the office is closing
            for (int k = 0; k < (int)numWait; k++)  // Wake up those couriers waiting for jobs
            {
                waitForReq.signal();
            }
            letGo.wait();
            break;
        }
        or _Accept(create, transfer)
        {
            // If state == 'C', msg of creation rendezvous complete
            // If state == 'T', msg of transfer rendezvous complete
            prt.print(Printer::WATCardOffice, state, nextStudent, nextAmount);
            if (numWait > 0)                        // if there are couriers waiting for jobs
            {
                waitForReq.signal();                // Wake up one courier waiting for a job
                letGo.wait();                       // let the courier that we just woke up go to finish jobs
            }
        }
        or _Accept(requestWork)
        {
            prt.print(Printer::WATCardOffice, 'W'); // Msg of courier rendezvous complete
        }
    }
}

//======================================================================================================
// Constructor for WATCardOffice
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    prt(prt), bank(bank), numCouriers(numCouriers)
{
    done = false;
    numWait = 0;                                                // No couriers are currently waiting
    courierArray = new WATCardOffice::Courier*[numCouriers];    // Create the couriers
    for (unsigned int k = 0; k < numCouriers; k++)
    {
        courierArray[k] = new WATCardOffice::Courier(prt, bank, this, k);
    }
}

//======================================================================================================
// Destructor for WATCardOffice
WATCardOffice::~WATCardOffice()
{
    for (int k = 0; k < (int)numCouriers; k++)
    {
        delete courierArray[k];
    }
    delete[] courierArray;
    
    // Delete all the futures jobs
    while (!requestsCopy.empty()) {                 // delete all jobs that the offices ever received
        Job *temp = requestsCopy.front();
        requestsCopy.pop_front();                   // pop it from the history (requestCopy)
        delete temp;                                // delete it
    }
    prt.print(Printer::WATCardOffice, 'F');         // Finished msg
}

//======================================================================================================
// A student calls this to asynchronously create a WATCard with an initial balance
// A future WATCard is returned and sufficient funds are subsequently obtained from the bank via a courier
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount )
{
    nextStudent = sid;                  // Copy student id for later process
    nextAmount = amount;                // Copy amount for later process
    nextCard = new WATCard();           // Create new WATCard
    state = 'C';                        // State of creation rendezvous complete
    
    Job *j = new Job(nextStudent, nextAmount, nextCard);    // Create job request
    requests.push_back(j);              // Add to list of requests
    requestsCopy.push_back(j);
    return j->result;                   // Return future in request
}

//======================================================================================================
// A student asynchronously calls this to tranfer money to his WATCard
// A future WATCard is returned and sufficient funds are subsequently obtained form the bank via a courier
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card )
{
    nextStudent = sid;                  // Copy student id for later process
    nextAmount = amount;                // Copy amount for later process
    nextCard = card;                    // Copy WATCard for later process
    state = 'T';                        // State of transfer rendezvous complete
    
    Job *j = new Job(nextStudent, nextAmount, nextCard);    // Create job request
    requests.push_back(j);              // Add to list of requests
    requestsCopy.push_back(j);
    return j->result;                   // Return future in request
}

//======================================================================================================
// Each courier task calls this to request a job
// It blocks until Job request is ready, and then receives the next Job as the result of the call
WATCardOffice::Job * WATCardOffice::requestWork()
{
    
    if (requests.empty())
    {
        numWait += 1;
        waitForReq.wait();          // Wait if no ready requests
        numWait -= 1;
        
        if (done) {
            letGo.signal();         // wake up the office
            throw WATCardOffice::Courier::Stop();   // Throw an Stop exception to stop the courier task
                                                    //   if office is closing down
        }
    }
    letGo.signal();                 // wake up the office

    Job *j = requests.front();      // Get the next job
    requests.pop_front();
    return j;
}
