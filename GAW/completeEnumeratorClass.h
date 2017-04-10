#ifndef COMPLETEENUMERATORCLASS_H
#define COMPLETEENUMERATORCLASS_H

#pragma once

#include <vector>

#include "GASolutionClass.h"

class completeEnumeratorClass
{
public:
	completeEnumeratorClass(void);
	~completeEnumeratorClass(void);

	void doEnumeration();
	
	void generateIncreaseByOne(int number, vector<unsigned int>& Array, int numberStop);
	void permutePumps();
	void permutePipesAndLinks(unsigned int position);

	std::vector< GASolutionClass >* targetSolutions;

private:
	
	vector<unsigned int> notation;
	vector< vector<unsigned int> > pipeLinks;
	vector< vector<unsigned int> > pumpPipeCombo;
	vector< vector<unsigned int> > pumpNumber;
	unsigned int numberOfPipeType;
	unsigned int numberOfLinks;
	unsigned int numberOfPumpType;
	unsigned int numberOfPumpsUse;
};

#endif