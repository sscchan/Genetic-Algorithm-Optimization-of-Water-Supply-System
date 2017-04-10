#ifndef PUMPCLASS_H
#define PUMPCLASS_H

#pragma once

class pumpClass
{
public:
	pumpClass(void);
	~pumpClass(void);
	
	void setIndex(int newIndex);
	int getIndex();

	void setID(std::string newID);
	std::string getID();

	double getCarbonCost();
	double getMonetaryCost();
	
	void updateEPANETCalculatedParameter();
	void updateEPANETDesignParameter();
	
	void downloadEPANETDesignParameter();

	void setPumpType(int newPumpType);
	int getPumpType();
	
	float getEnergyUsuage();
	float getCostPerDay();

	int getHeadFlowCurve();
	int getEffeciencyFlowCurve();
	
	void setHeadFlowCurve(int newHeadFlowCurve);
	void setEffeciencyFlowCurve(int newEffeciencyFlowCurve);

private:

	int p_Index;
	std::string p_ID;
	int p_PumpType;

	float p_EnergyUsage;
	float p_CostPerDay;
	
	int p_HeadFlowCurve;
	int p_EffeciencyFlowCurve;

};

#endif