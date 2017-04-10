#ifndef PIPETYPECLASS_H
#define PIPETYPECLASS_H

#pragma once

#include <string>

class pipeTypeClass
{
public:
	pipeTypeClass(void);
	~pipeTypeClass(void);
	
	std::string pipeName;
	double diameter;
	double perUnitLengthCarbonCost;
	double perUnitLengthMonetaryCost;
	double roughness;
};

#endif