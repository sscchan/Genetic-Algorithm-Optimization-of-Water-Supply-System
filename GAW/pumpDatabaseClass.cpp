#include "StdAfx.h"
#include ".\pumpdatabaseclass.h"
#include "EPAInterfaceClass.h"

#include <vector>
#include <string>
pumpDatabaseClass::pumpDatabaseClass(void)
{
}

pumpDatabaseClass::~pumpDatabaseClass(void)
{
}

pumpDatabaseClass* pumpDatabaseClass::_instance = 0;

pumpDatabaseClass* pumpDatabaseClass::Instance(){
	if (_instance == 0)
		_instance = new pumpDatabaseClass;
	return _instance;
}

/**
 * Determines the pumpTypes from the curves in EPANET
 * note: require the head flow curve ID be affixed "H"
 *		and require the effeciency flow curve ID be affixed "E"
 *	ie. PUMP-CEN18H
 *		PUMP-CEN18E
 *	denotes the two curves of a pump called PUMP-CEN18
 */
// checked 15-08-2006 3:17AM

void pumpDatabaseClass::loadPumpDatabase(){
	EPAInterfaceClass EPAInterface;
	
	std::vector<std::string> pumpCurveID;
	
	//EPANET starts all indexes with 1, push_back a placeholder
	pumpCurveID.push_back("");
	
	int i;
	for (i = 1; i <= EPAInterface.getNumberOfPumpCurves(); i++)
		pumpCurveID.push_back(EPAInterface.getCurveID(i));
	
	std::vector<int> indexOfHeadFlowCurves;
	std::vector<int> indexOfEffeciencyFlowCurves;
	

	for (i = 1; i < pumpCurveID.size(); i++)
	{
		unsigned int positionOfLastCharacter;
		positionOfLastCharacter = pumpCurveID.at(i).length() - 1;

		if( (pumpCurveID.at(i).substr(positionOfLastCharacter) == "p") || (pumpCurveID.at(i).substr(positionOfLastCharacter) == "P") )
			indexOfHeadFlowCurves.push_back(i);

		if(pumpCurveID.at(i).substr(positionOfLastCharacter) == "e" || pumpCurveID.at(i).substr(positionOfLastCharacter) == "E")
			indexOfEffeciencyFlowCurves.push_back(i);
	}
	
	//Remove the last character of each curve ID
	for (i = 1; i < pumpCurveID.size(); i++)
		if(pumpCurveID.at(i).empty() == false)
		pumpCurveID.at(i).erase(pumpCurveID.at(i).length() - 1);

	//Loop and seach for matching pairs of curves
	for (i = 0; i < indexOfHeadFlowCurves.size(); i++){
		for (int k = 0; k < indexOfEffeciencyFlowCurves.size(); k++)
			if(pumpCurveID.at(indexOfHeadFlowCurves.at(i)) == pumpCurveID.at(indexOfEffeciencyFlowCurves.at(k))){
				pumpTypeClass bufferPumpType;
				bufferPumpType.pumpName = pumpCurveID.at(indexOfHeadFlowCurves.at(i));
				bufferPumpType.effeciencyFlowCurve = indexOfEffeciencyFlowCurves.at(k);
				bufferPumpType.headFlowCurve = indexOfHeadFlowCurves.at(i);
				pumpTypes.push_back(bufferPumpType);
			}
	}
}


unsigned int pumpDatabaseClass::getNumberOfPumps()
{
	return pumpTypes.size();
}

std::string pumpDatabaseClass::getNameOfPump(int pumpIndex){
	return pumpTypes.at(pumpIndex).pumpName;
}
int pumpDatabaseClass::getHeadFlowCurveOfPump(int pumpIndex)
{
	return pumpTypes.at(pumpIndex).headFlowCurve;
}

int pumpDatabaseClass::getEffeciencyFlowCurveOfPump(int pumpIndex)
{
	return pumpTypes.at(pumpIndex).effeciencyFlowCurve;
}

void pumpDatabaseClass::printPumpTypes(){
	for(int i = 0; i < pumpTypes.size(); i++){
		std::cout << "PName: " << getNameOfPump(i) << " PID: " << i << " FC-Curves: "
				<< getHeadFlowCurveOfPump(i)
				<< " EF-Curves " << getEffeciencyFlowCurveOfPump(i) << "\n";
	}
}