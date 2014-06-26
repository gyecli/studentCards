//
//  watcard.h
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#ifndef ____watcard__
#define ____watcard__

#include <uC++.h>
#include <uFuture.h>

class WATCard {
    unsigned int money;                     // Current amount of money in the card
    
    WATCard( const WATCard & );             // prevent copying
    WATCard &operator=( const WATCard & );
public:
    WATCard();
    // FWATCard is a future pointer to a studnet's WATCard for synchronizing access
    // to the WATCard b/t studnet and courier
    typedef Future_ISM<WATCard *> FWATCard;
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

#endif /* defined(____watcard__) */
