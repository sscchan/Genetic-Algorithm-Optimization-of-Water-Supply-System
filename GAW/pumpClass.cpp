#include "StdAfx.h"
#include ".\pumpclass.h"
#include "pumpDatabaseClass.h"
#include "epanet2.h"
#include "programConfigurationClass.h"
#include <cmath>

pumpClass::pumpClass(void)
{
}

pumpClass::~pumpClass(void)
{
}

void pumpClass::setIndex(int newIndex){
	p_Index = newIndex;
}

int pumpClass::getIndex(){
	return p_Index;
}

void pumpClass::setID(std::string newID){
	p_ID = newID;
}

std::string pumpClass::getID(){
	return p_ID;
}

void pumpClass::setPumpType(int newPumpType){
	pumpDatabaseClass* pumpDatabase = pumpDatabaseClass::Instance();
	p_PumpType = newPumpType;
	
	p_HeadFlowCurve = pumpDatabase->getHeadFlowCurveOfPump(p_PumpType);
	p_EffeciencyFlowCurve = pumpDatabase->getEffeciencyFlowCurveOfPump(p_PumpType);

}

int pumpClass::getPumpType(){
	return p_PumpType;
}

double pumpClass::getCarbonCost(){
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();
	double carbonCost = p_EnergyUsage * programConfiguration->carbonPerKwHr * 365 * programConfiguration->pumpServiceLife;
	return carbonCost;
}

double pumpClass::getMonetaryCost(){
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();

	double r = programConfiguration->discountRate;
	double n = programConfiguration->pumpServiceLife;
	double P = getCostPerDay() * 365;

	double NPV;
	NPV = (1-std::pow(1+r,-1*n)) * P / r;

	return NPV;
}

float pumpClass::getCostPerDay(){
	return p_CostPerDay;
}

int pumpClass::getHeadFlowCurve(){
	return p_HeadFlowCurve;
}

int pumpClass::getEffeciencyFlowCurve(){
	return p_EffeciencyFlowCurve;
}

void pumpClass::setHeadFlowCurve(int newHeadFlowCurve){
	p_HeadFlowCurve = newHeadFlowCurve;
}

void pumpClass::setEffeciencyFlowCurve(int newEffeciencyFlowCurve){
	p_EffeciencyFlowCurve = newEffeciencyFlowCurve;
}

void pumpClass::updateEPANETDesignParameter(){
	EPAInterfaceClass EPAInterface;
	EPAInterface.setPumpHeadFlowCurve(getIndex(), p_HeadFlowCurve);
	EPAInterface.setPumpEffeciencyFlowCurve(getIndex(), p_EffeciencyFlowCurve);
}

void pumpClass::updateEPANETCalculatedParameter(){
	EPAInterfaceClass EPAInterface;
	
	//get average kW
	p_EnergyUsage = EPAInterface.getPumpEnergyUsage(getIndex());

	//get costPerDay
	p_CostPerDay = EPAInterface.getPumpCostPerDay(getIndex());
}

void pumpClass::downloadEPANETDesignParameter(){
	EPAInterfaceClass EPAInterface;
	p_ID = EPAInterface.getLinkID(p_Index);
}