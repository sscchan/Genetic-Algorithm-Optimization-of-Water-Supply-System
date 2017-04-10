#include "StdAfx.h"
#include ".\networkclass.h"
#include "EPAInterfaceClass.h"
#include "epanet2.h"
#include "programConfigurationClass.h"
#include <cassert>

networkClass::networkClass(void)
{
}

networkClass::~networkClass(void)
{
}

double networkClass::getTotalCarbonCost()
{
	return this->p_TotalCarbonCost;

}

double networkClass::getTotalMonetaryCost()
{
	return this->p_TotalMonetaryCost;

}

double networkClass::getTotalPressureViolation(){
	return p_TotalPressureViolation;
}

double networkClass::getTotalVelocityViolation(){
	return p_TotalVelocityViolation;
}

/**
 * Obtain the total pressure violation (using default acceptable range)
 * note: takes the default pressure boundary from programConfigurationClass
 */

//checked 16-08-2006 1:01PM
double networkClass::getPressureViolation(){
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();

	return this->getPressureViolation(programConfiguration->lowerPressureLimit,programConfiguration->upperPressureLimit);
}

/**
 * Returns the total pressure violation (values above and below the acceptable range) for the network
 * 
 * @param lowerPressureLimit		lower boundary of acceptable pressure range
 * @param upperPressureLimit		upper boundary of acceptable pressure range
 *
 * @return							the resulting violation
 */

// checked: 12-08-2006 3:49PM

double networkClass::getPressureViolation(double lowerPressureLimit, double upperPressureLimit){
	
	// Value holder for total pressure violation
	double totalPressureViolation_;
	totalPressureViolation_ = 0;

	// Counter
	unsigned int i;

	for (i = 0; i < nodes.size(); i++){
		
		double nodePressure = nodes[i].getPressure();
		
		// If overpressured
		if (nodePressure > upperPressureLimit)
			totalPressureViolation_ += (nodePressure - upperPressureLimit);
		else
			// If underpressured
			if (nodePressure < lowerPressureLimit)
				totalPressureViolation_ += (lowerPressureLimit - nodePressure);
	}

	return totalPressureViolation_;
}

/**
 * obtain the sum of all the velocity violation (using default boundary values)
 */

//chcekded 16-08-2006 1:07PM

double networkClass::getVelocityViolation()
{
	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();

	return this->getVelocityViolation(programConfiguration->lowerVelocityLimit,programConfiguration->upperVelocityLimit);
}

/**
 * obtain the sum of all the velocity violations (value below and above acceptable values) of a network
 * 
 * @param lowerVelocityLimit		lowest boundary for acceptable velocity
 * @param upperVelocityLimit		upper boundary for acceptable velocity
 *
 * @return							the total velocity violation
 */

//checked 12-08-2006 3:41 PM

double networkClass::getVelocityViolation(double lowerVelocityLimit, double upperVelocityLimit){

	// Value holder for total velocity violation
	double totalVelocityViolation_;
	totalVelocityViolation_ = 0;

	// Counter
	unsigned int i;

	for (i = 0; i < pipes.size(); i++){
		
		double pipeVelocity = pipes[i].getVelocity();

		// If velocity too high
		if (pipeVelocity > upperVelocityLimit)
			totalVelocityViolation_ += abs(upperVelocityLimit - pipeVelocity);
		else
			// If velocity too low
			if (pipeVelocity < lowerVelocityLimit)
				totalVelocityViolation_ += abs(lowerVelocityLimit - pipeVelocity);
	}

	return totalVelocityViolation_;
}

/**
 * Downloads the general structure and design parameters of the network loaded into EPANET.
 */

//checked: 12-08-2006 3:24PM
void networkClass::loadFromEPANET(){

	//clears the pipes, pumps and nodes data from the object
	pipes.clear();
	nodes.clear();
	pumps.clear();

	//init the interface
	EPAInterfaceClass EPAInterface;
	
	//download the node information
	unsigned int numberOfNodes;
	numberOfNodes = EPAInterface.getNumberOfNodes();
	
	nodeClass bufferNode;

	unsigned int i;
	for (i = 0; i < numberOfNodes; i++){
		//loop through all nodes and test if they're junctions
		//note: EPA reference starts at 1, therefore i+1
		if (EPAInterface.getNodeType(i+1) == EN_JUNCTION){
			bufferNode.setIndex(i+1);
			bufferNode.downloadEPANETDesignParamter();
			nodes.push_back(bufferNode);
		}
	}
	
	//download the link information
	//note: pumps and pipes are treated as "links" in EPANET
	unsigned int numberOfLinks;
	numberOfLinks = EPAInterface.getNumberOfLinks();

	pipeClass bufferPipe;
	pumpClass bufferPump;

	for (i = 0; i < numberOfLinks; i++){
		if (EPAInterface.getLinkType(i+1) == EN_PIPE){
			bufferPipe.setIndex(i+1);
			bufferPipe.downloadEPANETDesignParameter();
			pipes.push_back(bufferPipe);
		}
		if (EPAInterface.getLinkType(i+1) == EN_PUMP){
			bufferPump.setIndex(i+1);
			bufferPump.downloadEPANETDesignParameter();
			pumps.push_back(bufferPump);
		}
	}

	

}

// This function upload all the design parameters to EPANET
void networkClass::loadToEPANET(){
	// Counter variable
	unsigned int i;

	for (i = 0; i < pipes.size(); i++){
		pipes[i].updateEPANETDesignParameter();
	}

	for (i = 0; i < nodes.size(); i++){
		nodes[i].updateEPANETDesignParameter();
	}

	for (i = 0; i < pumps.size(); i++){
		pumps[i].updateEPANETDesignParameter();
	}
}

// This function obtain all of the calculated data from EPANET and place
// them in the <vector> or pipes, nodes and pumps
void networkClass::updateEPANETCalculatedParameter(){
	// Counter variable
	unsigned int i;

	for (i = 0; i < pipes.size(); i++){
		pipes[i].updateEPANETCalculatedParameter();
	}

	for (i = 0; i < nodes.size(); i++){
		nodes[i].updateEPANETCalculatedParameter();
	}

	for (i = 0; i < pumps.size(); i++){
		pumps[i].updateEPANETCalculatedParameter();
	}	
}

void networkClass::calculateDesignVariables(){
	//Update all the parameters
	this->loadToEPANET();

	programConfigurationClass* programConfiguration = programConfigurationClass::Instance();
	
	this->p_TotalCarbonCost = 0;
	this->p_TotalMonetaryCost = 0;
	this->p_TotalPressureViolation = 0;
	this->p_TotalVelocityViolation = 0;
	
	unsigned int i = 0;

	////Include captial carbon and monetary cost
	//for (unsigned int i = 0; i < pipes.size(); i++)
	//	p_TotalMonetaryCost = p_TotalMonetaryCost + pipes[i].getMonetaryCost();
	//
	//for (i = 0; i < pipes.size(); i++)
	//	p_TotalCarbonCost = p_TotalCarbonCost + pipes[i].getCarbonCost();
	//
	//Loop through the timesteps
	long timeStep;
	do{
		int errorCode;

		errorCode = ENrunH(&timeStep);
		assert(errorCode < 100);

		updateEPANETCalculatedParameter();

		p_TotalPressureViolation = p_TotalPressureViolation + this->getPressureViolation();
		p_TotalVelocityViolation = p_TotalVelocityViolation + this->getVelocityViolation();
		
		errorCode = ENnextH(&timeStep);
		assert(errorCode < 100);
	} while (timeStep > 0);
	
	updateEPANETCalculatedParameter();

	for (i = 0; i < pumps.size(); i++){
		p_TotalCarbonCost = p_TotalCarbonCost + pumps.at(i).getCarbonCost();
	}

	for (i = 0; i < pumps.size(); i++){
		p_TotalMonetaryCost = p_TotalMonetaryCost + pumps.at(i).getMonetaryCost();
	}
}

//Debug Functions
void networkClass::printNetwork()
{
	//Print Nodes
	std::cout << "NODE Information" << "\n";
	cout << "=====================================" << "\n";
	for (int i = 0; i < this->nodes.size(); i++){
		cout << "Index: " << nodes.at(i).getIndex() 
			 << " ID: " << nodes.at(i).getID()
			 << " Demand: " << nodes.at(i).getBaseDemand()
			 << " Head: " << nodes.at(i).getHead()
			 << " Pressure: " << nodes.at(i).getPressure()
			 << "\n";
	}
	
	cout << "\n";

	//Print Links
	cout << "PIPE Information" << "\n";
	cout << "=====================================" << "\n";
	for (int i = 0; i < this->pipes.size(); i++){
		cout << "Index: " << pipes.at(i).getIndex()
			<< " ID: " << pipes.at(i).getID()
			<< " Length: " << pipes.at(i).getLength()
			<< " Diameter: " << pipes.at(i).getDiameter()
			<< " Roughness: " << pipes.at(i).getRoughness()
			<< " Velocity: " << pipes.at(i).getVelocity()
			<< " Flow: " << pipes.at(i).getFlow()
			<< " Headloss: " << pipes.at(i).getHeadloss()
			<< "\n";
	}

	cout << "\n";

	//Print Pumps
	cout << "PUMP Information" << "\n";
	cout << "=====================================" << "\n";
	for (int i = 0; i < this->pumps.size(); i++){
		cout << "Index: " << pumps.at(i).getIndex()
			<< " ID: " << pumps.at(i).getID()
			<< " HF-Curve: " << pumps.at(i).getHeadFlowCurve()
			<< " EF-Cruve: " << pumps.at(i).getEffeciencyFlowCurve()
			<< " CostPerDay: " <<pumps.at(i).getCostPerDay()
			<< " KwhPerDay: " << pumps.at(i).getCarbonCost()
			<< "\n";

	}
}