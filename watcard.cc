//
//  watcard.cc
//  
//
//  Created by Guotian Tim Ye and Taiyue Barry Liu on 11/22/2013.
//
//

#include "watcard.h"


//======================================================================================================
// Constructor for WATCard
WATCard::WATCard()
{
    money = 0;          // When WATCard is first created, it has no money in it
}

//======================================================================================================
// Courier calls this after a funds tranfer
void WATCard::deposit( unsigned int amount )
{
    money += amount;    // Add to the account
}

//======================================================================================================
// A vending machine calls this when a soda is purchased
void WATCard::withdraw( unsigned int amount )
{
    money -= amount;    // Withdraw from the account
}

//======================================================================================================
// A student and a vending machine call this to determine the balance in the WATCard
unsigned int WATCard::getBalance()
{
    return money;
}
