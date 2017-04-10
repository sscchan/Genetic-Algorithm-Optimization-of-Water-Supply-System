#ifndef NODECLASS_H
#define NODECLASS_H

#pragma once
#include <string>

class nodeClass
{
public:
	nodeClass(void);
	nodeClass(int index, std::string ID, float elevation, float baseDemand);
	~nodeClass(void);

	void setIndex(int newIndex);
	int getIndex();

	void setID(std::string newID);
	std::string getID();

	void setElevation(float newElevation);
	float getElevation();

	void setBaseDemand(float newBaseDemand);
	float getBaseDemand();

	float getPressure();
	float getHead();
	
	// Macro to update the design and calculated parameters
	void updateEPANETDesignParameter();
	void updateEPANETCalculatedParameter();
	void downloadEPANETDesignParamter();

private:
	
	// Private "set" functions for EPANET calculated values
	void setPressure(float newPressure);
	void setHead(float newHead);

	int p_Index;
	std::string p_ID;
	
	// Node Design Parameters
	float p_Elevation;
	float p_BaseDemand;
	
	// EPANET Calculated Values
	float p_Pressure;
	float p_Head;
	
	
};

#endif