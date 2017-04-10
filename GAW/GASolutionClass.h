#ifndef GASOLUTIONCLASS_H
#define GASOLUTIONCLASS_H

#pragma once
#include "networkclass.h"
#include <vector>
#include <bitset>


class GASolutionClass :
	public networkClass
{
	friend class MOGAClass;
	friend class completeEnumeratorClass;
public:
	GASolutionClass(void);
	~GASolutionClass(void);

	bool isDominate(GASolutionClass otherSolution);

	void encodeChromosome();
	void decodeChromosome();

	void mutateChromosome();
	void mutateChromosome(std::vector< std::bitset<1> > *targetChromosome, double mutationProbability);
	void recombineChromosome(std::vector< std::bitset<1> > firstTargetChromosome,
		std::vector< std::bitset<1> > secondTargetChromosome,
		std::vector< std::vector < std::bitset<1> > > *childrenChromosomes);
	
	void randomizeSolution();
	//Debug Functions Only
	void debugPrintBitVector(vector< bitset<1> > targetBitVector);

private:
	int p_nonDominationRank;
	double p_crowdingDistance;
	
	unsigned int p_dominationCount;
	std::vector<unsigned int> p_solutionsDominated;

	std::vector< std::bitset<1> > p_chromosome;

	unsigned int bitVectorToInt(std::vector< std::bitset<1> > subjectBitVector);
	std::vector< std::bitset<1> > intToBitVector(unsigned int subjectInt);
};

#endif