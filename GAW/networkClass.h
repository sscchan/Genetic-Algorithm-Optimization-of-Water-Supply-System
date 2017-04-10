#pragma once
#ifndef NETWORKCLASS_H
#define NETWORKCLASS_H

#include <vector>

#include "pipeClass.h"
#include "nodeClass.h"
#include "pumpClass.h"

using namespace std;

class networkClass
{
public:
	networkClass(void);
	~networkClass(void);

	double getTotalCarbonCost();
	double getTotalMonetaryCost();
	
	// The total magnitude of the networks' violation from operating guidelines.
	double getPressureViolation();
	double getPressureViolation(double lowerPressureLimit, double upperPressureLimit);
	double getVelocityViolation();
	double getVelocityViolation(double lowerVelocityLimit, double upperVelocityLimit);
	
	double getTotalPressureViolation();
	double getTotalVelocityViolation();

	// loading the network from and to the EPANET
	void loadToEPANET(void);
	void loadFromEPANET(void);

	// *** WARNING, EPANET must be deployed and ran ***
	void updateEPANETCalculatedParameter();
	
	void calculateDesignVariables();

	//Debug Functions
	void printNetwork();

protected:
	vector<pipeClass> pipes;
	vector<nodeClass> nodes;
	vector<pumpClass> pumps;

	double p_TotalCarbonCost;
	double p_TotalMonetaryCost;
	double p_TotalPressureViolation;
	double p_TotalVelocityViolation;
};

#endif