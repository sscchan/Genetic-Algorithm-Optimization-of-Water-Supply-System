
#include "StdAfx.h"
#include ".\completeenumeratorclass.h"
#include "networkClass.h"
#include "GASolutionClass.h"
#include "pumpDatabaseClass.h"
#include "pipeDatabaseClass.h"
#include <iostream> //Input Output Stream
#include <vector>
#include <math.h>
#include <fstream> //File Input Output Stream
#include <sstream> //StringStream 
#include <string>  //String Class
#include "epanet2.h"
#include <cassert>

using namespace std;

completeEnumeratorClass::completeEnumeratorClass(void)
{
}

completeEnumeratorClass::~completeEnumeratorClass(void)
{
}


void completeEnumeratorClass::doEnumeration(){
	//Obtain network configurations
	
	EPAInterfaceClass EPAInterface;
	unsigned int pipeCount = 0;
	unsigned int pumpCount = 0;
	for (unsigned int i = 1; i <= EPAInterface.getNumberOfLinks(); i++){
		if (EPAInterface.getLinkType(i) == EN_PIPE)
			pipeCount++;
		if (EPAInterface.getLinkType(i) == EN_PUMP)
			pumpCount++;
	}

	numberOfLinks = pipeCount;
	
	pumpDatabaseClass* pumpDatabase = pumpDatabaseClass::Instance();
	numberOfPumpType = pumpDatabase->getNumberOfPumps();
	
	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::Instance();
	numberOfPipeType = pipeDatabase->getNumberOfPipeTypes();

	numberOfPumpsUse = pumpCount;

	//Obtain the sequences
	notation.resize(numberOfLinks,0);
	unsigned int position=numberOfLinks-1;

	permutePumps();
	permutePipesAndLinks(position);
	
	//Debug: Print Out Enumerator Vectors
	for (size_t comboIndex = 0; comboIndex < pumpPipeCombo.size(); comboIndex++){
		for (size_t cElementIndex = 0; cElementIndex < pumpPipeCombo.at(comboIndex).size(); cElementIndex++)
			std::cout << pumpPipeCombo.at(comboIndex).at(cElementIndex) << " ";
		std::cout << endl;
	}

	//Erases compoent vectors to free memory
	pipeLinks.clear();
	pumpNumber.clear();
	notation.clear();

	//Create GANetworks as per the sequences
	targetSolutions->clear();

	GASolutionClass bufferClass;
	bufferClass.loadFromEPANET();
	
	//Check if the enumerated length is correct
	assert(pumpPipeCombo.size() > 0);
	assert(pumpPipeCombo.at(0).size() == bufferClass.pipes.size() + bufferClass.pumps.size());

	for (size_t comboCounter = 0; comboCounter < pumpPipeCombo.size(); comboCounter++){
		unsigned int enumeratorVectorPosition = 0;
		for (size_t pumpCounter = 0; pumpCounter < bufferClass.pumps.size(); pumpCounter++){
			bufferClass.pumps.at(pumpCounter).setPumpType(pumpPipeCombo.at(comboCounter).at(enumeratorVectorPosition));
			enumeratorVectorPosition++;
		}
		for (size_t pipeCounter = 0; pipeCounter < bufferClass.pipes.size(); pipeCounter++){
			bufferClass.pipes.at(pipeCounter).setPipeType(pumpPipeCombo.at(comboCounter).at(enumeratorVectorPosition));
			enumeratorVectorPosition++;
		}
		
		bufferClass.updateEPANETCalculatedParameter();
		bufferClass.calculateDesignVariables();
		bufferClass.printNetwork();
		targetSolutions->push_back(bufferClass);
	}
	
	//Clear memory
	pumpPipeCombo.clear();
}

//Class for generating permutations by increasing each entry by one
void completeEnumeratorClass::generateIncreaseByOne(int number,vector<unsigned int>& Array,int numberStop)
{
   if(number <0)
    return;

  Array.at(number) = (Array.at(number) + 1)%numberStop;
  if(Array.at(number) == 0)
    generateIncreaseByOne(number-1,Array,numberStop);
  
}


void completeEnumeratorClass::permutePumps()
{
  vector<unsigned int> Pump(numberOfPumpsUse,0);
  int Length = (int)pow((long)numberOfPumpType,(long)numberOfPumpsUse);
  for(int p=0;p<Length; p++)
    {
      pumpNumber.push_back(Pump);
      generateIncreaseByOne(Pump.size()-1,Pump,numberOfPumpType);
    }

}


void completeEnumeratorClass::permutePipesAndLinks(unsigned int position)
{
  unsigned int Total_Pumps = pumpNumber.size();
  unsigned long int totalNumberOfIterations = (unsigned long int)pow((long)numberOfPipeType,(long)numberOfLinks);
     
  for(unsigned int p =0;p<Total_Pumps;p++)
    {
      for(unsigned int r=0;r<pumpNumber.at(p).size();r++)
      
        for(unsigned long int i=0;i<totalNumberOfIterations;i++)
	{
	  
		pipeLinks.push_back(notation);
		generateIncreaseByOne(notation.size()-1,notation,numberOfPipeType);
		
		vector<unsigned int> bufferCombo;
		bufferCombo.clear();

		 for(unsigned int r=0;r<pumpNumber.at(p).size();r++)
		   {			 
			 bufferCombo.push_back(pumpNumber.at(p).at(r));
		   }
		 
		for(int j=0;j<(int)notation.size();j++)
		  {			
			  bufferCombo.push_back(pipeLinks.at(i).at(j));
		   }
			
		  pumpPipeCombo.push_back(bufferCombo);
		 
	}
 }
}