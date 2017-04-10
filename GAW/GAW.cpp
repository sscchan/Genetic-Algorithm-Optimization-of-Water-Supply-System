// GAW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "networkClass.h"
#include "EPAInterfaceClass.h"
#include "epanet2.h"
#include "assert.h"
#include "GASolutionClass.h"
#include "MOGAClass.h"
#include <string>
#include "pumpDatabaseClass.h"
#include "pipeDatabaseClass.h"
#include "programConfigurationClass.h"

/**
 * Program Command
 * 
 * GAW inputFileLocation pipeDatabaseLocation initialPopulationNumber, populationNumber, numberOfGenerations
 */

int _tmain(int argc, _TCHAR* argv[])
{
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();
	
	programConfiguration->startEPANET();

	MOGAClass MOGA;
	
	programConfiguration->terminateEPANET();
	return 0;
}

