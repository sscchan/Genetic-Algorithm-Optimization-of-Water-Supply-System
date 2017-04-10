#ifndef MOGACLASS_H
#define MOGACLASS_H

#pragma once
#include "GASolutionClass.h"
#include <string>
class MOGAClass
{
public:
	MOGAClass(void);
	~MOGAClass(void);
private:
	void initialization();
	
	void mutation();
	void reproduction();
	
	void selection();
	
	bool isSolutionEqual(GASolutionClass firstSolution, GASolutionClass secondSolution);
	
	void printResults();

	vector<GASolutionClass> GASolutions;
	void nonDominatedSort();
	void crowdingDistanceAssignment(vector<unsigned int> front);
	bool partialOrder(unsigned int leftSolutionIndex, unsigned int rightSolutionIndex);

	vector< vector<unsigned int> > nonDominatedFronts;

	//Objective Comparision (for sorting)
	
	void quickSort(std::vector<unsigned int>* indexVector, std::string objectiveType, unsigned int left, unsigned int right);
	unsigned int qs_partition(vector<unsigned int>* indexVector, std::string objectiveType, unsigned int left, unsigned int right, unsigned int pivotIndex);
	
	double qs_getValue(unsigned int atIndex, string objectiveType);
	void qs_swapValue(std::vector<unsigned int>* subjectVector, unsigned int firstIndex, unsigned int secondIndex);
	
	//debug
	void qs_printIntVector(std::vector<unsigned int>* subjectVector);
};

#endif