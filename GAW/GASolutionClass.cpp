#include "StdAfx.h"
#include <cstdlib>
#include <ctime>
#include ".\gasolutionclass.h"
#include "pipeDatabaseClass.h"
#include "pumpDatabaseClass.h"
#include "assert.h"

// Debug Functions
void GASolutionClass::debugPrintBitVector(vector< bitset<1> > targetBitVector){
		for (int i = 0; i < targetBitVector.size(); i++)
			cout << targetBitVector.at(i).at(0);
		cout << "\n";
	}

GASolutionClass::GASolutionClass(void)
{
	//randomize seed
	//TODO: Improve randomization
	//TODO: Move to a central Initization Location
	std::srand( (unsigned)time ( NULL ) );

	this->p_crowdingDistance = 0;
	this->p_dominationCount = 0;
	this->p_nonDominationRank = 0;
	this->p_solutionsDominated.clear();

}

GASolutionClass::~GASolutionClass(void)
{
}

/**
 * checks if this solution dominates another
 *
 * @param otherSolution		the other solution under question
 * return					true / false
 */

//Checked 12-08-2006 2:32 PM

bool GASolutionClass::isDominate(GASolutionClass otherSolution){
	
	//check if the two solutions are equal (if so, return notDominated)
	if (this->getTotalCarbonCost() == otherSolution.getTotalCarbonCost())
		if (this->getTotalMonetaryCost() == otherSolution.getTotalMonetaryCost())
			if (this->getTotalPressureViolation() == otherSolution.getTotalPressureViolation())
				if (this->getTotalVelocityViolation() == otherSolution.getTotalVelocityViolation())
					return false;
	
	//check if this dominates the other
	if (this->getTotalCarbonCost() <= otherSolution.getTotalCarbonCost())
		if (this->getTotalMonetaryCost() <= otherSolution.getTotalMonetaryCost())
			if (this->getTotalPressureViolation() <= otherSolution.getTotalPressureViolation())
				if (this->getTotalVelocityViolation() <= otherSolution.getTotalVelocityViolation())
					return true;
	
	//if above not true, must be mixed in superiority, non-dominating
	return false;

}

/**
 *Mutates the chromosome of this solution
 */
void GASolutionClass::mutateChromosome(){
	this->mutateChromosome(&p_chromosome, 1/p_chromosome.size());
}

/**
 * Mutate a vector of bits (chromosome / gene). 
 * This function will mutate each bit of the bit vector with the probability specified.
 *
 * @param *targetChromosome		pointer to the vector of bits under question
 * @param mutationProbability	the probability of each bit being mutated
 */

//Checked 12/08/2006 - 1:31PM

void GASolutionClass::mutateChromosome(std::vector< std::bitset<1> > *targetChromosome, double mutationProbability){
	
	//asserts the mutationProbability is within range (0 <= mutationProbability <= 1);
	//the different range accounts for double's inaccuracy
	assert(mutationProbability >= -0.1);
	assert(mutationProbability <= 1.1);

	//for all bits...
	unsigned int i;
	for (i = 0; i < targetChromosome->size(); i++){
		
		//if random number below probability therehold, mutate the specific bit
		if ( (float) std::rand() / RAND_MAX <= mutationProbability)
			targetChromosome->at(i).flip(0);
	}
}

/**
 * Recombine a vector of bits (chromosome / gene) at a random location and return 2 children.
 *
 * @param firstTargetChromosome		the first parent
 * @param secondTargetChromosme		the second parent
 * @param *childrenChromosomes		pointer to a vector of vector of bits
 */

//Checked 12/08/2006 - 1:57PM
//TODO: Improvement - provision for multiple parents, multiple crossover points

void GASolutionClass::recombineChromosome(std::vector< std::bitset<1> > firstTargetChromosome, std::vector< std::bitset<1> > secondTargetChromosome, std::vector< std::vector < std::bitset<1> > > *childrenChromosomes){

	//asserts the two target chromosome has same length
	assert(firstTargetChromosome.size() == secondTargetChromosome.size());

	//randomly select a crossover point
	unsigned long crossoverPoint;
	crossoverPoint = (int) firstTargetChromosome.size() * rand() / (RAND_MAX + 1.0);
	
	//create firstChild
	std::vector < std::bitset<1> > firstChild;
	firstChild = secondTargetChromosome;
	for (unsigned long i = crossoverPoint; i < secondTargetChromosome.size(); i++)
		firstChild.at(i) = firstTargetChromosome.at(i);
	childrenChromosomes->push_back(firstChild);
	
	//create secondChild
	std::vector < std::bitset<1> > secondChild;
	secondChild = firstTargetChromosome;
	for (unsigned long i = crossoverPoint; i < secondTargetChromosome.size(); i++)
		secondChild.at(i) = secondTargetChromosome.at(i);
	childrenChromosomes->push_back(secondChild);

}

/**
 * Encodes the this solution into a bitstring (chromosome)
 */
//Checked 15-08-2006 2:44 AM

void GASolutionClass::encodeChromosome(){

	//Clear the chromosome
	p_chromosome.clear();

	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::Instance();

	unsigned int numberOfPipeTypes = pipeDatabase->getNumberOfPipeTypes();
		
	unsigned int pipeBitSize;
	pipeBitSize = intToBitVector(numberOfPipeTypes -1).size();
	
	unsigned int i;
	for (i = 0; i < this->pipes.size(); i++){
		std::vector< bitset<1> > bufferBitVector;
		bufferBitVector = intToBitVector(pipes.at(i).getPipeType());
		
		//Push 0 bits if the size of the bufferVector is less than what it's suppose to be
		if(bufferBitVector.size() < pipeBitSize){
			for (int j = 0; j < pipeBitSize - bufferBitVector.size(); j++)
				p_chromosome.push_back( (std::bitset<1>) 0);
		}

		for (unsigned int k = 0; k < bufferBitVector.size(); k++)
			p_chromosome.push_back(bufferBitVector.at(k));
	}
	
	pumpDatabaseClass* pumpDatabase = pumpDatabaseClass::Instance();

	unsigned int pumpBitSize;
	pumpBitSize = intToBitVector(pumpDatabase->getNumberOfPumps() - 1).size();

	for (i = 0; i < this->pumps.size(); i++){
		std::vector < bitset<1> > bufferBitVector;
		bufferBitVector = intToBitVector(pumps.at(i).getPumpType());

		if (bufferBitVector.size() < pumpBitSize){
			for (int j = 0; j < pumpBitSize - bufferBitVector.size(); j++)
				p_chromosome.push_back( (std::bitset<1>) 0);
		}

		for (unsigned int k = 0; k < bufferBitVector.size(); k++)
			p_chromosome.push_back(bufferBitVector.at(k));
	}
}


/**
 * Decodes the chromosome and updates the internal data
 */
// checked: 15-08-2006 2:56AM

void GASolutionClass::decodeChromosome(){
	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::Instance();

	unsigned int numberOfPipeTypes = pipeDatabase->getNumberOfPipeTypes();
	
	//obtain the width of the gene for each pipe
	unsigned int pipeBitSize;
	pipeBitSize = intToBitVector(numberOfPipeTypes -1).size();
	
	for (unsigned int i = 0; i < pipes.size(); i++){
		unsigned int decodedPipeType;
		
		//extract the gene from the chromosome
		std::vector< std::bitset<1> > pipeTypeGene;
		for (unsigned int k = i * pipeBitSize; k < (i+1) * pipeBitSize; k++){
			pipeTypeGene.push_back(p_chromosome.at(k));
		}
		
		//convert bitstring (gene) into integer
		decodedPipeType = bitVectorToInt(pipeTypeGene);
	
		//check if the decoded value is wihin bounds
		while (decodedPipeType > pipeDatabase->getNumberOfPipeTypes() - 1)
		{
			//if not within bounds, mutate the gene until it is
			//mutation is set at 2 / genesize
			this->mutateChromosome(&pipeTypeGene, 2 / pipeTypeGene.size());
			decodedPipeType = bitVectorToInt(pipeTypeGene);
		}
		
		//assign the decoded value to the pipe
		pipes.at(i).setPipeType(decodedPipeType);
	}

	

	pumpDatabaseClass* pumpDatabase = pumpDatabaseClass::Instance();

	unsigned int pumpBitSize;
	pumpBitSize = intToBitVector(pumpDatabase->getNumberOfPumps() - 1).size();
	
	for (unsigned int i = 0; i < pumps.size(); i++){
		std::vector < std::bitset<1> > pumpTypeGene;

		unsigned int startingPosition = pipeBitSize * pipes.size();
		
		//TODO: test if the position is correct
		for (unsigned int k = startingPosition + i * pumpBitSize; k < startingPosition + (i+1) * pumpBitSize; k++)
		{
			pumpTypeGene.push_back(p_chromosome.at(k));
		}

		unsigned int decodedPumpType = bitVectorToInt(pumpTypeGene);

		//check if pumpType exceeds existing pumpType and change appropriately
		while (decodedPumpType > pumpDatabase->getNumberOfPumps() - 1){
			this->mutateChromosome(&pumpTypeGene, 5 / pumpTypeGene.size());
			decodedPumpType = bitVectorToInt(pumpTypeGene);
		}

		pumps.at(i).setPumpType(decodedPumpType);
	}
}

/**
 * Converts a vector of bits to a Integer (unsigned)
 *
 * @param subjectBitVector		the bit vector in question
 * @return						the resulting integer
 */

//Checked 12-08-2006 2:01 PM

unsigned int GASolutionClass::bitVectorToInt(std::vector< std::bitset<1> > subjectBitVector){
	unsigned int bufferSum;
	bufferSum = 0;

	unsigned int i;
	for (i = 0; i < subjectBitVector.size() ; i++){
		bufferSum = bufferSum * 2 + int(subjectBitVector.at(i).at(0));
	}
	
	return bufferSum;
}

/**
 * Converts an (unsigned) Integer to a bit vector
 *
 * @param subjectInt		the integer under question
 * @return					resulting vector of bits
 */

//Checked 12-08-2006 2:02 PM

std::vector < std::bitset<1> > GASolutionClass::intToBitVector(unsigned int subjectInt){
	
	//declares a buffer vector of bits (required by algorithm)
	std::vector< std::bitset<1> > bufferVector;
	
	//declares a buffer integer for the algorihtm
	int subjectIntBuffer;
	subjectIntBuffer = subjectInt;

	while (subjectIntBuffer > 0){
		bufferVector.push_back(subjectIntBuffer % 2);
		subjectIntBuffer = subjectIntBuffer / 2;
	}
	
	//declares the actual answer vector
	std::vector< std::bitset<1> > answerVector;
	
	//reverses the buffer bitstring, and store into answer bitstring
	int i;	
	for (i = bufferVector.size() - 1; i > -1; i--){
		answerVector.push_back(bufferVector.at(i));
	}
	
	return answerVector;
}


void GASolutionClass::randomizeSolution(){
	pumpDatabaseClass* pumpDatabase = pumpDatabaseClass::Instance();
	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::Instance();

	//Randomize Pipes
	for (unsigned int i = 0; i < pipes.size(); i++){
		double randomNumber = ((double)rand() / RAND_MAX) * (pipeDatabase->getNumberOfPipeTypes() - 1);
		pipes.at(i).setPipeType((unsigned int) randomNumber);
	}

	for (unsigned int i = 0; i < pumps.size(); i++){
		double randomNumber = ((double)rand() / RAND_MAX) * (pumpDatabase->getNumberOfPumps() - 1);
		pumps.at(i).setPumpType((unsigned int) randomNumber);
	}
}