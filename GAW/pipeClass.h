#ifndef PIPECLASS_H
#define PIPECLASS_H
#pragma once

class pipeClass
{
public:
	pipeClass(void);
	pipeClass(int index, std::string ID, unsigned int pipeType, float length, float roughness);
	~pipeClass(void);
	
	///////////////////////
	// Designe Parameters
	//////////////////////

	void setIndex(int newIndex);
	int getIndex();

	void setID(std::string newID);
	std::string getID();
	
	// PipeType (Corresponding to the PipeDatabase)
	void setPipeType(unsigned int newPipeType);
	unsigned int getPipeType();

	void setDiameter(float newDiameter);
	float getDiameter();

	void setLength(float newLength);
	float getLength();

	void setRoughness(float newRoughness);
	float getRoughness();
	
	/////////////////////////
	// Calculated Parameters
	/////////////////////////
	// Note: some parameters can only be calculated by EPANET, hence some "set" functions are
	//       in the private section
	////////////////////////

	float getFlow();
	float getVelocity();
	float getHeadloss();
	
	// Values not calculated by EPANET
	float getCarbonCost();
	float getMonetaryCost();

	// Update the Design Paramters to EPANET
	void updateEPANETDesignParameter();

	// Obtain the Calculated Values from EPANET
	void updateEPANETCalculatedParameter();
	
	// Obtain basic values from EPANET
	void downloadEPANETDesignParameter();

private:

	// Private Calculated Parameter "set" functions
	void setFlow(float newFlow);
	void setVelocity(float newVelocity);
	void setHeadloss(float newHeadloss);


	///////////////////////
	// Private Variables
	///////////////////////
	std::string p_ID;
	int p_Index;

	unsigned int p_PipeType;

	float p_Diameter;
	float p_Length;
	float p_Roughness;

	float p_Flow;
	float p_Velocity;
	float p_Headloss;
};

#endif