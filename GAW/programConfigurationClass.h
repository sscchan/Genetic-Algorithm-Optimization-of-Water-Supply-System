#ifndef PROGRAMCONFIGURATIONCLASS_H
#define PROGRAMCONFIGURATIONCLASS_H
#pragma once

class programConfigurationClass
{
public:
	static programConfigurationClass* Instance();
	
	//GA Parameters
	unsigned long initialPopulationSize;
	unsigned long populationSize;
	unsigned long numberOfGenerations;

	//TODO: Find Default Values
	double lowerVelocityLimit;
	double upperVelocityLimit;

	double lowerPressureLimit;
	double upperPressureLimit;

	//Time Parameters (in hrs)
	int simulationDuration;
	int hydraulicTimeStep;
	int numberOfTimeSteps;
	
	void startEPANET();
	void runAnalysis();
	void terminateEPANET();
	
	char* EPANETInputLocation;
	char* pipeDatabaseInputLocation;
	
	//Pump Carbon & monetary cost calculations
	double discountRate;
	double carbonPerKwHr;
	double pumpServiceLife;

protected:
	programConfigurationClass(void);
	~programConfigurationClass(void);

	static programConfigurationClass* _instance;
};

#endif