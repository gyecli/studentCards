//
//  watcardOffice.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____watcardOffice__
#define ____watcardOffice__

#include <uC++.h>
#include <uFuture.h>
#include <list>
#include "watcard.h"

_Monitor Printer;       // Forward declarations
_Monitor Bank;

_Task WATCardOffice {
    struct Job {                            // marshalled arguments and return future
        unsigned int sid;                   // Student id
        unsigned int amount;                // Amount of money needed to transfer
        WATCard *watcard;                   // WATCard that is invloved in the job
        WATCard::FWATCard result;           // return future
        Job( unsigned int id, unsigned int amount, WATCard *watcard_ ) : sid(id), amount(amount) { watcard=watcard_;}
    };
    
    // *****************************************
    // communicates with bank
    _Task Courier {
        Printer &prt_;          // Printer
        Bank &bank_;            // Bank
        WATCardOffice *office;  // WATCard office
        unsigned id_;           // Courier id
        
        void main();            // Courier main
        
    public:
        _Event Stop {};
        Courier( Printer &prt_, Bank &bank_, WATCardOffice *office, unsigned int id_);
        ~Courier();
    };
    // ****************************************
    
    Printer &prt;               // Printer
    Bank &bank;                 // Bank
    int done;                   // Indicate the office is closing down
    unsigned int numCouriers;   // Number of couriers
    
    unsigned int nextStudent;   // Next student that WATCard office is about to serve
    unsigned int nextAmount;    // Amount of cash that the office is about to process
    unsigned int numWait;       // Number of couriers that are waiting for requests
    char state;                 // State that needs to print by WATCardOffice
    WATCard::FWATCard *fCardArray;          // Array of FWATCard
    WATCard *nextCard;          // Next WATCard about to serve
    WATCardOffice::Courier **courierArray;  // Array of couriers
    std::list<Job *> requests;  // List of requests
    std::list<Job *> requestsCopy;          // Used for memory deallocation
    uCondition waitForReq;      // Condition on which couriers wait for requests
    uCondition letGo;
    
    void main();                // WATCardOffice main
    
public:
    _Event Lost {};             // uC++ exception type, like "struct"
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif /* defined(____watcardOffice__) */
