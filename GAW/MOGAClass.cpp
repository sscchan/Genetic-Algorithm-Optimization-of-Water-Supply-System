#include "StdAfx.h"
#include ".\mogaclass.h"
#include "GASolutionClass.h"
#include <algorithm>
#include <string>
#include "assert.h"
#include "programConfigurationClass.h"
#include "completeEnumeratorClass.h"
#include <vector>
#include <bitset>
#include <cmath>

MOGAClass::MOGAClass(void)
{
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();
	completeEnumeratorClass completeEnumerator;
	
	initialization();
	
	completeEnumerator.targetSolutions = &GASolutions;
	completeEnumerator.doEnumeration();
	printResults();

	for (unsigned long generationCount = 0; generationCount < programConfiguration->numberOfGenerations; generationCount++){

		mutation();	


		//Crossover Solutions
		
		reproduction();


		//Selection

		//Calculate Population Fitness values
		for (unsigned int i = 0; i < GASolutions.size(); i++){
			GASolutions.at(i).calculateDesignVariables();
		}

		//Debug: (for display only) Calculate Population Fitness values
		for (unsigned int i = 0; i < GASolutions.size(); i++){
			GASolutions.at(i).calculateDesignVariables();
		}

		std::cout <<"G: "<< generationCount <<"\n";
		printResults();

		//Sort according to fitness
		nonDominatedSort();

		selection();

		//For each solutions
		//for (unsigned long i = 0; i < GASolutions.size(); i++){
		//	std::cout << "Solution: " << i << "\n";
		//	std::cout << "------------------------" << "\n";
		//	std::cout << "MonetaryCost: " << GASolutions.at(i).getTotalMonetaryCost() << "\n";
		//	std::cout << "CarbonCost: " << GASolutions.at(i).getTotalCarbonCost() << "\n";
		//	std::cout << "VelocityViolation: " << GASolutions.at(i).getTotalVelocityViolation() << "\n";
		//	std::cout << "PressureViolation: " << GASolutions.at(i).getPressureViolation() << " " << "\n";

		//	std::cout << "\nPipe \n";
		//	std::cout << "linkIndex pipeType \n";
		//	for (unsigned int k = 0; k < GASolutions.at(i).pipes.size(); k++)
		//		std::cout << GASolutions.at(i).pipes.at(k).getIndex() << " " << GASolutions.at(i).pipes.at(k).getPipeType() << "\n";
		//	
		//	std::cout << "\nPump \n";
		//	std::cout << "pumpIndex pumpType \n";
		//	for (unsigned int k = 0; k < GASolutions.at(i).pumps.size(); k++)
		//		std::cout << GASolutions.at(i).pumps.at(k).getIndex() << " " << GASolutions.at(i).pumps.at(k).getPumpType() << "\n";	
		//	std::cout << "\n";
		//}

		//Debug: (for display only) Calculate Population Fitness values
		for (unsigned int i = 0; i < GASolutions.size(); i++){
			GASolutions.at(i).calculateDesignVariables();
		}

		std::cout <<"G: "<< generationCount <<"\n";
		printResults();
	}

}

void MOGAClass::initialization(){
	
	//Clear exisiting solutions
	this->GASolutions.clear();

	//Init programConfiguration singleton
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();
	
	//Establish Initial Population

	//Establsih buffer GASolutionClass
	GASolutionClass bufferSolution;
	bufferSolution.loadFromEPANET();
	
	//Create randomized solutions
	for (unsigned long populationCounter = 0; populationCounter < programConfiguration->initialPopulationSize; populationCounter++){
		bufferSolution.randomizeSolution();
		bufferSolution.encodeChromosome();
		GASolutions.push_back(bufferSolution);
	}
}

void MOGAClass::mutation(){
	for (unsigned long solutionCounter = 0; solutionCounter < GASolutions.size(); solutionCounter++){
		GASolutions.at(solutionCounter).mutateChromosome();
		GASolutions.at(solutionCounter).decodeChromosome();
		GASolutions.at(solutionCounter).encodeChromosome();
	}
}

void MOGAClass::reproduction(){
	size_t parentPopulation = GASolutions.size();

	for (size_t solutionCounter = 0; solutionCounter < parentPopulation; solutionCounter++){
		std::vector< std::bitset<1> > selfChromosome = GASolutions.at(solutionCounter).p_chromosome;

		//Obtain random partner
		double randomNumber = (double) std::rand()/RAND_MAX * (parentPopulation - 1);
		unsigned long randomPartner = (unsigned long) randomNumber;
		std::vector< std::bitset<1> > partnerChromosome = GASolutions.at(randomPartner).p_chromosome;
		
		std::vector< std::vector< std::bitset<1> > > childrenChromosomes;

		GASolutions.at(solutionCounter).recombineChromosome(selfChromosome, partnerChromosome, &childrenChromosomes);
		
		//Put children into the set of GASolutions
		GASolutionClass bufferSolution;
		bufferSolution.loadFromEPANET();

		for (size_t childrenCounter = 0; childrenCounter < childrenChromosomes.size(); childrenCounter++){
			bufferSolution.p_chromosome = childrenChromosomes.at(childrenCounter);
			bufferSolution.decodeChromosome();
			bufferSolution.encodeChromosome();
			GASolutions.push_back(bufferSolution);
		}

	}
}

void MOGAClass::selection(){
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();

	//Remove non-unique solutions from the fronts
	
	//for each nonDominatedFront...
	for (unsigned long nonDominatedFrontCounter = 0; nonDominatedFrontCounter < nonDominatedFronts.size(); nonDominatedFrontCounter++){
		std::vector<unsigned int> uniqueFront;
		uniqueFront.clear();
		
		//for each element within each front
		for (unsigned long frontElementCounter = 0; frontElementCounter < nonDominatedFronts.at(nonDominatedFrontCounter).size(); frontElementCounter++){
			unsigned int elementValue = nonDominatedFronts.at(nonDominatedFrontCounter).at(frontElementCounter);
			
			//First solution is always unique
			if (frontElementCounter == 0){
				uniqueFront.push_back(elementValue);
			}
			else{
				//Test for uniqueness
				bool isUnique = true;				
				for (size_t i = 0; i < uniqueFront.size(); i++){
					if (isSolutionEqual(GASolutions.at(elementValue), GASolutions.at(uniqueFront.at(i))))
						isUnique = false;
				}

				if (isUnique)
					uniqueFront.push_back(elementValue);
			}
		}
		
		//Replace the front with the unique front
		nonDominatedFronts.at(nonDominatedFrontCounter) = uniqueFront;
	}

	//Generate new generation
	std::vector< GASolutionClass > newGeneration;
	

	//Fill the newGeneration with all those within the nonDominated Front
	for (unsigned long q = 0; q < nonDominatedFronts.size(); q++){
		for (unsigned long w = 0; w < nonDominatedFronts.at(q).size(); w++){
			if (newGeneration.size() < programConfiguration->populationSize)
				newGeneration.push_back(GASolutions.at(nonDominatedFronts.at(q).at(w)));
		}
	}
	
	//If there's not enough unique solutions
	if (newGeneration.size() < programConfiguration->populationSize){
		unsigned long populationDeficit = programConfiguration->populationSize - newGeneration.size();
		GASolutionClass bufferSolution;
		bufferSolution.loadFromEPANET();
		
		//Generate random solutions and put into population
		for (unsigned long e = 0; e < populationDeficit; e++){
			bufferSolution.randomizeSolution();
			bufferSolution.encodeChromosome();
			newGeneration.push_back(bufferSolution);
		}
	}
	
//Replace the old generation with the new one
	GASolutions = newGeneration;
}

void MOGAClass::printResults(){
			
		for (unsigned long i = 0; i < GASolutions.size(); i++){
			std::cout << "Solution: " << i;
			std::cout << " " << GASolutions.at(i).getTotalMonetaryCost();
			std::cout << " " << GASolutions.at(i).getTotalCarbonCost();
			std::cout << " " << GASolutions.at(i).getTotalVelocityViolation();
			std::cout << " " << GASolutions.at(i).getPressureViolation();
			std::cout << " CD: " << GASolutions.at(i).p_crowdingDistance << " ";

			std::cout << " PI: ";

			for (unsigned int k = 0; k < GASolutions.at(i).pipes.size(); k++)
				std::cout << GASolutions.at(i).pipes.at(k).getPipeType() << " ";

			std::cout << " PU: ";

			for (unsigned int k = 0; k < GASolutions.at(i).pumps.size(); k++)
				std::cout << GASolutions.at(i).pumps.at(k).getPumpType() << " ";	
			std::cout << "\n";
		}
}
bool MOGAClass::isSolutionEqual(GASolutionClass firstSolution, GASolutionClass secondSolution){
	firstSolution.encodeChromosome();
	secondSolution.encodeChromosome();
	return (firstSolution.p_chromosome == secondSolution.p_chromosome);
}
MOGAClass::~MOGAClass(void)
{
}

/**
 * Sorts the set of GASolutions into non-dominant fronts (containing the indexes to GASolutions)
 */
// checked 15-08-2006 4:31AM

void MOGAClass::nonDominatedSort()
{	
	nonDominatedFronts.clear();

	unsigned int i;
	
	vector<unsigned int> bufferFront;
	bufferFront.clear();

	for (i = 0; i < GASolutions.size(); i++){
		
		unsigned int j;
		
		for (j = 0; j < GASolutions.size(); j++){
			if (i != j){
				if (GASolutions[i].isDominate(GASolutions[j]))
					GASolutions[i].p_solutionsDominated.push_back(j);
				else
					if (GASolutions[j].isDominate(GASolutions[i]))
						GASolutions[i].p_dominationCount++;
			}
		}

		if (GASolutions[i].p_dominationCount == 0){
			GASolutions[i].p_nonDominationRank = 0;
			bufferFront.push_back(i);
		}
	}
	
	nonDominatedFronts.push_back(bufferFront);

	// nonDominated Front Counter
	i = 0;

	while (nonDominatedFronts.size() > i){
		bufferFront.clear(); //buffer for the next front

		// for each member of the current front...
		unsigned int j;
		for (j = 0; j < nonDominatedFronts[i].size(); j++){
			
			// for each member of the dominated solution of the member of the current front
			unsigned int k;
			for (k = 0; k < GASolutions[nonDominatedFronts[i].at(j)].p_solutionsDominated.size(); k++)
			{
				int dominatedSolutionIndex = GASolutions.at(nonDominatedFronts.at(i).at(j)).p_solutionsDominated.at(k);
				GASolutions[dominatedSolutionIndex].p_dominationCount--;
				if (GASolutions[dominatedSolutionIndex].p_dominationCount == 0){
					GASolutions[dominatedSolutionIndex].p_nonDominationRank = i + 1;
					bufferFront.push_back(dominatedSolutionIndex);
				}
			}
		}

		if (bufferFront.empty() == false)
			nonDominatedFronts.push_back(bufferFront);
		i++;
	}
	
	//debug print nonDominated Front
	//for (int i = 0; i < nonDominatedFronts.size(); i++){
	//	for (int k = 0; k < nonDominatedFronts.at(i).size(); k++)
	//		//GASolutions.at(nonDominatedFronts.at(i).at(k)).printNetwork();
	//		std::cout << nonDominatedFronts.at(i).at(k) << " ";
	//	std::cout << "\n" << "--------------" << "\n";
	//}
}

/**
 * Partial Order (as per defined in Deb)
 *
 * @param leftSolutionIndex		the index of the GASolution on the left
 * @param rightSolutionIndex	the index of the GASolution on thr right
 *
 * @return						true / false
 */

// checked 15-08-2006

bool MOGAClass::partialOrder(unsigned int leftSolutionIndex, unsigned int rightSolutionIndex)
{
	if (GASolutions[leftSolutionIndex].p_nonDominationRank < GASolutions[rightSolutionIndex].p_nonDominationRank)
		return true;
	if (GASolutions[leftSolutionIndex].p_nonDominationRank == GASolutions[rightSolutionIndex].p_nonDominationRank)
		if (GASolutions[leftSolutionIndex].p_crowdingDistance > GASolutions[rightSolutionIndex].p_crowdingDistance)
			return true;
	return false;
}

/**
 * Obtain the value of a GASolution, given it's index and the objective
 * 
 * @param atIndex			Index of GASolutions
 * @param objectiveType		String of the Objective
 * @return					The value of the objective
 */
//checked 16-08-2006 12:38 AM

double MOGAClass::qs_getValue(unsigned int atIndex, std::string objectiveType)
{
	if (objectiveType == "monetaryCost")
		return this->GASolutions.at(atIndex).getTotalMonetaryCost();
	if (objectiveType == "carbonCost")
		return this->GASolutions.at(atIndex).getTotalCarbonCost();
	if (objectiveType == "velocityViolation")
		return this->GASolutions.at(atIndex).getTotalVelocityViolation();
	if (objectiveType == "pressureViolation")
		return this->GASolutions.at(atIndex).getTotalPressureViolation();
	if (objectiveType == "crowdingDistance")
		return this->GASolutions.at(atIndex).p_crowdingDistance;
	
	// one should never reach here
	assert(false);

	return 0;
}

/**
 * Swap the value of a vector of unsigned int at two locations
 * (part of the quicksort algorithm)
 */
// checked 16-08-2006 112:45AM

void MOGAClass::qs_swapValue(std::vector<unsigned int> *subjectVector, unsigned int firstIndex, unsigned int secondIndex){
	unsigned int buffer;
	buffer = subjectVector->at(firstIndex);
	subjectVector->at(firstIndex) = subjectVector->at(secondIndex);
	subjectVector->at(secondIndex) = buffer;
}

unsigned int MOGAClass::qs_partition(std::vector<unsigned int> *indexVector, std::string objectiveType, unsigned int left, unsigned int right, unsigned int pivotIndex){
	
	
	//unsigned int pivotValue = qs_getValue(pivotIndex, objectiveType);
	//qs_swapValue(indexVector, pivotIndex, right);
	//
	//unsigned int storeIndex = left;

	//int i;
	//for (i = left; i < right; i++){
	//	
	//	qs_printIntVector(indexVector);

	//	if(qs_getValue(i, objectiveType) <= pivotValue){
	//		qs_swapValue(indexVector, storeIndex, i);
	//		storeIndex++;
	//	}
	//}

	//qs_swapValue(indexVector, right, storeIndex);

	//qs_printIntVector(indexVector);
	//return storeIndex;
	return 0;
}


/**
 * Sorts an index vector according to some objective value in GASolutions
 */
// checked 16-08-2006 2:40AM

void MOGAClass::quickSort(std::vector <unsigned int> *indexVector, std::string objectiveType, unsigned int left, unsigned int right){
	//Don't know why it doesn't work...
	//if(left < right){
	//	unsigned int chosenPivotIndex;
	//	chosenPivotIndex = (right - left) / 2 + left;
	//	unsigned int newPivotIndex;
	//	newPivotIndex = qs_partition(indexVector, objectiveType, left, right, chosenPivotIndex);
	//	quickSort(indexVector, objectiveType, left, newPivotIndex -1);
	//	quickSort(indexVector, objectiveType, newPivotIndex + 1, right);
	//}

	//BubbleSort
	
	bool isSorted;
	do{
		isSorted = true;
		int n = 1;
		for (unsigned long i = 0; i < indexVector->size() - n; i++)
		{
			if(qs_getValue(indexVector->at(i), objectiveType) > qs_getValue(indexVector->at(i + 1), objectiveType)){
				qs_swapValue(indexVector, i, i+1);
				isSorted = false;
			}
		}

	} while (isSorted == false);
}

/**
 * Debug Function - prints a vector<unsigned int>
 */
// checked 16-08-2006 12:49 AM

void MOGAClass::qs_printIntVector(std::vector<unsigned int> *subjectVector){
	for (unsigned int i = 0; i < subjectVector->size(); i++)
		std::cout << subjectVector->at(i) << " ";
	std::cout << "\n";
}

/** 
 * Assigns the crowding distance for each GA Solution
 *
 * @param front		the index of the GASolutions within a front
 */

void MOGAClass::crowdingDistanceAssignment(vector<unsigned int> front)
{
	unsigned int i;
	
	//Reset crowdig distance
	for (i = 0; i < front.size(); i++)
		GASolutions[front[i]].p_crowdingDistance = 0;
	
	//Vector of solutions at extremes of the objectives
	vector<unsigned int> extremeObjectiveSolutions;

	////Monetary Objective

	////Sort, ascending order
	quickSort(&front, "monetaryCost", 0, front.size() -1);
	
	//Obtain Max & Min
	double minObjectiveValue;
	double maxObjectiveValue;
	
	minObjectiveValue = GASolutions.at(front[0]).getTotalMonetaryCost();
	maxObjectiveValue = GASolutions.at(front[front.size() - 1]).getTotalMonetaryCost();
	
	//Record extreme points
	extremeObjectiveSolutions.push_back(front[0]);
	extremeObjectiveSolutions.push_back(front[front.size() - 1]);

	//For points between the extreme points
	for (i = 1; i < front.size() -1; i++){
		if (minObjectiveValue < maxObjectiveValue)
			GASolutions[front[i]].p_crowdingDistance += (GASolutions[front[i + 1]].getTotalMonetaryCost() - GASolutions[front[i - 1]].getTotalMonetaryCost()) / (maxObjectiveValue - minObjectiveValue);
	}

	//Carbon Objective

	//Sort, ascending order
	quickSort(&front, "carbonCost", 0, front.size() -1);

	//Obtain Max & Min
	minObjectiveValue = GASolutions.at(front[0]).getTotalCarbonCost();
	maxObjectiveValue = GASolutions.at(front[front.size() - 1]).getTotalCarbonCost();

	//Record extreme points
	extremeObjectiveSolutions.push_back(front[0]);
	extremeObjectiveSolutions.push_back(front[front.size() - 1]);
	
	//For points between the extreme points
	for (i = 1; i < front.size() -1; i++){
		if (minObjectiveValue < maxObjectiveValue)
			GASolutions[front[i]].p_crowdingDistance += (GASolutions[front[i + 1]].getTotalCarbonCost() - GASolutions[front[i - 1]].getTotalCarbonCost()) / (maxObjectiveValue - minObjectiveValue);
	}

	//Velocity Objective

	//Sort, ascending order
	quickSort(&front, "velocityViolation", 0, front.size() -1);

	//Obtain Max & Min
	minObjectiveValue = GASolutions.at(front[0]).getTotalVelocityViolation();
	maxObjectiveValue = GASolutions.at(front[front.size() - 1]).getTotalVelocityViolation();

	//Record extreme points
	extremeObjectiveSolutions.push_back(front[0]);
	extremeObjectiveSolutions.push_back(front[front.size() - 1]);
	
	//For points between the extreme points
	for (i = 1; i < front.size() -1; i++){
		if (minObjectiveValue < maxObjectiveValue)
			GASolutions[front[i]].p_crowdingDistance += (GASolutions[front[i + 1]].getTotalVelocityViolation() - GASolutions[front[i - 1]].getTotalVelocityViolation()) / (maxObjectiveValue - minObjectiveValue);
	}

	//Pressure Objective

	//Sort, ascending order
	quickSort(&front, "pressureViolation", 0, front.size() -1);

	//Obtain Max & Min
	minObjectiveValue = GASolutions.at(front[0]).getTotalPressureViolation();
	maxObjectiveValue = GASolutions.at(front[front.size() - 1]).getTotalPressureViolation();

	//Record extreme points
	extremeObjectiveSolutions.push_back(front[0]);
	extremeObjectiveSolutions.push_back(front[front.size() - 1]);
	
	
	//For points between the extreme points
	for (i = 1; i < front.size() -1; i++){
		if (minObjectiveValue < maxObjectiveValue)
			GASolutions[front[i]].p_crowdingDistance += (GASolutions[front[i + 1]].getTotalPressureViolation() - GASolutions[front[i - 1]].getTotalPressureViolation()) / (maxObjectiveValue - minObjectiveValue);
	}

	//Now let the extrme points have near infinite distance so they're always selected
	for (i = 0; i < extremeObjectiveSolutions.size(); i++){
		GASolutions[extremeObjectiveSolutions[i]].p_crowdingDistance = 99999;
	}
}
