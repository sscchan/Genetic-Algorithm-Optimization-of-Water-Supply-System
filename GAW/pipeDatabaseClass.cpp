#include "StdAfx.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

#include ".\pipedatabaseclass.h"
#include "pipeTypeClass.h"
#include "programConfigurationClass.h"


using namespace std;
pipeDatabaseClass* pipeDatabaseClass::_instance = 0;

pipeDatabaseClass* pipeDatabaseClass::Instance(){
	if (_instance == 0){
		_instance = new pipeDatabaseClass;
	}
	return _instance;
}
pipeDatabaseClass::pipeDatabaseClass(void)
{
	this->readFromFile();
}

pipeDatabaseClass::~pipeDatabaseClass(void)
{
}

void pipeDatabaseClass::readFromFile(){
	
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();

	pipeTypes.clear();

	pipeTypeClass bufferPipeType;

	char line[100];
	std::vector<float> num;
	char *single;
	int j=0;
	int k=1;
	int m=2;
	int n=3;
	fstream myfile(programConfiguration->pipeDatabaseInputLocation,std::ios::in);

	if(myfile.is_open())
	{
		while(!myfile.eof())
		{
			myfile.getline(line,1000);
		    single = strtok (line," ");

		
			while(single!=NULL)
			{
			num.push_back(atof(single));
			single=strtok(NULL," ");
			}
			
			
			while(num[j]!=NULL)
			{
			bufferPipeType.diameter=num[j];
			j=j+4;
			break;
            }
			
			while(num[k]!=NULL)
			{
			bufferPipeType.perUnitLengthCarbonCost=num[k];
			k=k+4;
			break;
            }
			
			while(num[m]!=NULL)
			{
			bufferPipeType.perUnitLengthMonetaryCost=num[m];
			m=m+4;
			break;
            }
			
			while(num[n]!=NULL)
			{
			bufferPipeType.roughness=num[n];
			n=n+4;
			break;
            }
			
			// Add the new data to the vector  
			pipeTypes.push_back(bufferPipeType);
		}			
		}
	
	//Remove the extra entry at the bottom
	//pipeTypes.pop_back();

	//Debug
	/*for (int i = 0; i < pipeTypes.size(); i++){
		std::cout << "Pipe #" << i << " D:" << pipeTypes.at(i).diameter
			<< " MonCost:" << pipeTypes.at(i).perUnitLengthMonetaryCost
			<< " CarCost:" << pipeTypes.at(i).perUnitLengthCarbonCost
			<< " Roughness:" << pipeTypes.at(i).roughness << "\n";

	}*/
}

double pipeDatabaseClass::getUnitLengthCarbonCost(unsigned int pipeTypeIndex)
{
	
	return pipeTypes[pipeTypeIndex].perUnitLengthCarbonCost;
}

double pipeDatabaseClass::getUnitLengthMonetaryCost(unsigned int pipeTypeIndex)
{

	return pipeTypes[pipeTypeIndex].perUnitLengthMonetaryCost;

}

double pipeDatabaseClass::getRoughness(unsigned int pipeTypeIndex)
{

		return pipeTypes[pipeTypeIndex].roughness;
}

double pipeDatabaseClass::getdiameter(unsigned int pipeTypeIndex)
{
	return pipeTypes[pipeTypeIndex].diameter;
}

unsigned int pipeDatabaseClass::getNumberOfPipeTypes(){
	return pipeTypes.size();
}