#include "StdAfx.h"
#include ".\programconfigurationclass.h"
#include "pipeDatabaseClass.h"
#include "pumpDatabaseClass.h"
#include "assert.h"
#include "epanet2.h"

programConfigurationClass::programConfigurationClass(void)
{
	initialPopulationSize = 20;
	populationSize = 20;
	numberOfGenerations = 10;

	lowerVelocityLimit = 0; //1
	upperVelocityLimit = 1000; //4

	lowerPressureLimit = 0; //1
	upperPressureLimit = 1000; //4

	simulationDuration = 24;
	hydraulicTimeStep = 2;
	numberOfTimeSteps = simulationDuration / hydraulicTimeStep;
	
	//Input file locations
	EPANETInputLocation = "c:\\EPATEST\\robmanualnetwork.inp";
	pipeDatabaseInputLocation = "c:\\EPATEST\\robtestpipedatabase.txt";
	
	//Pump Carbon & monetary cost calculations
	discountRate = 0.1;
	carbonPerKwHr = 0.970;
	pumpServiceLife = 50;
	

}

programConfigurationClass::~programConfigurationClass(void)
{
}

programConfigurationClass* programConfigurationClass::_instance = 0;

programConfigurationClass* programConfigurationClass::Instance(){
	if (_instance == 0){
		_instance = new programConfigurationClass;
	}
	return _instance;
}

void programConfigurationClass::startEPANET(){
	
	//Starts pipeDatabase and Load from File
	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::Instance();
	pipeDatabase->readFromFile();

	int errorCode;
	errorCode = ENopen(EPANETInputLocation, "someoutput.dat", "");
	assert(errorCode == 0);
	
	//Setup the timesteps, etc
	errorCode = ENsettimeparam(EN_DURATION, (long) simulationDuration * 60 * 60);
	assert(errorCode == 0);

	errorCode = ENsettimeparam(EN_HYDSTEP, (long) hydraulicTimeStep * 60 * 60);
	assert(errorCode == 0);

	errorCode = ENopenH();
	assert(errorCode == 0);

	errorCode = ENinitH(00);
	assert(errorCode == 0);
	
	//loads the pumpDatabase
	pumpDatabaseClass* pumpDatabase = pumpDatabaseClass::Instance();
	pumpDatabase->loadPumpDatabase();


}

void programConfigurationClass::runAnalysis(){
	// Defunct!
	//int errorCode;
	//long timeStep;
	//errorCode = ENrunH(&timeStep);
	//assert(errorCode == 0);
}

void programConfigurationClass::terminateEPANET(){

	int errorCode = ENclose();
	assert (errorCode == 0);
}