/*
	Zero Intelligence Traders
	
	Object-Oriented Version+
	
	Robert Axtell
	
	The Brookings Institution
	Middlebury College
		and
  George Mason University
	
	First version: October 1998
	Subsequent version: September 2004
  Current version: July 2009
  Updated for XCode 7 and OS 10.10: Fall 2015
	
	Reference: Gode and Sunder, QJE, 1993

 */
 
#include <iostream>
#include <math.h>
#include <time.h>

#include "RNG.h"
#include "Data.h"


//////////////////////////////////////////////
//
//	Constant, type and variable definitions...
//
//////////////////////////////////////////////

#define seed 1
#define seedRandomWithTime false
#if seedRandomWithTime
#define theSeed time(0)
#else
#define theSeed seed
#endif

const bool buyer = true;
const bool seller = false;

const int maxNumberOfTrades = 20000000;

//	Specify the number of agents of each type...
const int numberOfBuyers = 1000000;
const int numberOfSellers = 1000000;

//	Specify the maximum internal values...
const int maxBuyerValue = 20;
const int maxSellerValue = 20;

//	Define an agent...
class Agent
{
	bool buyerOrSeller;
	int quantityHeld;
	int value;
	int price;
public:
	Agent(bool agentType);
	bool GetBuyerOrSeller();
	int GetQuantityHeld();
	void SetQuantityHeld (int theQuantity);
	int FormBidPrice();
	int FormAskPrice();
	int GetPrice();
	void SetPrice (int thePrice);
};

typedef Agent *AgentPtr;

//	Define the model object and declare an instance...
class Model
{
	AgentPtr Buyers[numberOfBuyers];
	AgentPtr Sellers[numberOfSellers];
 public:
	Model();
	Data TradeData, PriceData;
	void OpenTrading();
	void DoTrades (int maxTrades);
};

RandomNumberGenerator RNG(theSeed);

/////////////////////////////
//
//	Object Implementations...
//
/////////////////////////////

Agent::Agent (bool agentType)
{
	if (agentType == buyer)
	{
		buyerOrSeller = buyer;
		quantityHeld = 0;
		value = RNG.IntegerInRange(1, maxBuyerValue);
	}
	else	// agentType == seller
	{
 		buyerOrSeller = seller;
 		quantityHeld = 1;
 		value = RNG.IntegerInRange(1, maxSellerValue);
	};
};	//	Agent::Agent()

bool Agent::GetBuyerOrSeller()
{
	return buyerOrSeller;
};	//	Agent:GetBuyerOrSeller()

int Agent::GetQuantityHeld()
{
	return quantityHeld;
};	//	Agent::GetQuantityHeld()

void Agent::SetQuantityHeld (int theQuantity)
{
	quantityHeld = theQuantity;
};	//	Agent::SetQuantityHeld()

int Agent::FormBidPrice()
{
	return RNG.IntegerInRange(1, value);
};	//	Agent::FormBidPrice()

int Agent::FormAskPrice()
{
	return RNG.IntegerInRange(value, maxBuyerValue);
};	//	Agent::FormAskPrice()

int Agent::GetPrice()
{
	return price;
};	//	Agent::GetPrice()

void Agent::SetPrice (int thePrice)
{
	price = thePrice;
};	//	Agent::SetPrice()

Model::Model()
{
//	Fill the agent fields...
 	
 	//	First the buyers...
 	for (int i=0; i<numberOfBuyers; i=i+1)
		Buyers[i] = new Agent(buyer);

	//	Now the sellers...
 	for (int i=0; i<numberOfSellers; i=i+1)
 		Sellers[i] = new Agent(seller);
 		
//	Now initialize the data objects...
	PriceData.Init();
	TradeData.Init();

};	//	Model::Model()

void Model::OpenTrading()
{
		DoTrades(maxNumberOfTrades);
};	//	Model::OpenTrading()

void Model::DoTrades (int maxTrades)
//
//	This method pairs agents at random and then selects a price randomly...
//
{
	int buyerIndex, sellerIndex;
	int bidPrice, askPrice, transactionPrice;
	
	for (int i=1; i<=maxTrades; i=i+1)
	{
	 	//	Pick a buyer at random, then pick a 'bid' price randomly between 1 and the agent's private value;
    //
		buyerIndex = RNG.IntegerInRange(0, numberOfBuyers-1);
		bidPrice = Buyers[buyerIndex]->FormBidPrice();
	 	
	 	//	Pick a seller at random, then pick an 'ask' price randomly between the agent's private value and maxSellerValue;
    //
		sellerIndex = RNG.IntegerInRange(0, numberOfSellers-1);
		askPrice = Sellers[sellerIndex]->FormAskPrice();
	 	
	 	//	Let's see if a deal can be made...
    //
	 	if ((Buyers[buyerIndex]->GetQuantityHeld() == 0) && (Sellers[sellerIndex]->GetQuantityHeld() == 1) && (bidPrice >= askPrice))
	 	{
	 		//	First, compute the transaction price...
      //
	 		transactionPrice = RNG.IntegerInRange(askPrice, bidPrice);
	 		Buyers[buyerIndex]->SetPrice(transactionPrice);
	 		Sellers[sellerIndex]->SetPrice(transactionPrice);
	 		PriceData.AddDatuum(transactionPrice);
	 		
	 		//	Then execute the exchange...
      //
	 		Buyers[buyerIndex]->SetQuantityHeld(1);
	 		Sellers[sellerIndex]->SetQuantityHeld(0);
	 		TradeData.AddDatuum(1);
	 	};
	};
};	//	Model::DoTrades()

///////////
//
//	MAIN...
//
///////////
 
int main()
{
	time_t startTime, endTime;
	double dif;

	using namespace std;
	cout << "\nZERO INTELLIGENCE TRADERS" << endl << endl;

	Model *ZITraders;
  ZITraders = new Model;
	
	time(&startTime);
	
	ZITraders->OpenTrading();
	
	time(&endTime);
	dif = difftime(endTime, startTime);
	
	cout << "The model took " << dif << " seconds to execute" << endl;
	cout << "Quantity traded = " << ZITraders->TradeData.GetN() << endl << endl;
	cout << "The average price = " << ZITraders->PriceData.GetAverage() << " and the s.d. is " << ZITraders->PriceData.GetStdDev() << endl;
}
