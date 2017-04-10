#ifndef PIPEDATABASECLASS_H
#define PIPEDATABASECLASS_H

#pragma once

#include <vector>
#include <string>
#include "pipeTypeClass.h"

class pipeDatabaseClass
{
public:
	static pipeDatabaseClass* Instance();

	// Reads the data from file
	void readFromFile();
	
	std::string getPipeTypeName(unsigned int pipeTypeIndex);
	double getdiameter(unsigned int pipeTypeIndex);
	double getUnitLengthCarbonCost(unsigned int pipeTypeIndex);
	double getUnitLengthMonetaryCost(unsigned int pipeTypeIndex);
	double getRoughness(unsigned int pipeTypeIndex);
	
	unsigned int getNumberOfPipeTypes();

protected:
	pipeDatabaseClass(void);
	~pipeDatabaseClass(void);

private:
	static pipeDatabaseClass* _instance;

	std::vector<pipeTypeClass> pipeTypes;
};

#endif
