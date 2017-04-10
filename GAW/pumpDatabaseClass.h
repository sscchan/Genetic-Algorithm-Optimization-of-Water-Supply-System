#ifndef PUMPDATABASECLASS_H
#define PUMPDATABASECLASS_H

#pragma once
#include <vector>
#include <string>

#include "pumpTypeClass.h"

class pumpDatabaseClass
{
public:
	static pumpDatabaseClass* Instance();

	unsigned int getNumberOfPumps();
	
	std::string getNameOfPump(int pumpIndex);
	int getHeadFlowCurveOfPump(int pumpIndex);
	int getEffeciencyFlowCurveOfPump(int pumptIndex);

	void loadPumpDatabase();
	
	//debug functions
	void printPumpTypes();

protected:
	pumpDatabaseClass(void);
	~pumpDatabaseClass(void);

private:
	static pumpDatabaseClass* _instance;
	std::vector<pumpTypeClass> pumpTypes;
};

#endif