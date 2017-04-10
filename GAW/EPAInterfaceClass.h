#ifndef EPAINTERFACECLASS_H
#define EPAINTERFACECLASS_H

#pragma once

class EPAInterfaceClass
{
public:
	EPAInterfaceClass(void);
	~EPAInterfaceClass(void);

	// Link Interfaces
	int getNumberOfLinks(void);

	int getLinkStartNode(int linkIndex);
	int getLinkEndNode(int linkIndex);
	
	int getLinkType(int linkIndex);
	std::string getLinkID(int linkIndex);
	double getLinkDiameter(int linkIndex);
	void setLinkDiameter(int linkIndex, float newDiameter);

	double getLinkLength(int linkIndex);
	void setLinkLength(int linkIndex, float newLength);

	double getLinkRoughness(int linkIndex);
    void setLinkRoughness(int linkIndex, float newRoughness);

	double getLinkFlow(int linkIndex);
	double getLinkVelocity(int linkIndex);
	double getLinkHeadloss(int linkIndex);

	// Node Interfaces
	int getNumberOfNodes(void);

	int getNodeType(int nodeIndex);
	bool isNodeReservior(int nodeIndex);

	std::string getNodeID(int nodeIndex);

	double getNodeElevation(int nodeIndex);
	void setNodeElevation(int nodeIndex, float newElevation);

	double getNodeBaseDemand(int nodeIndex);
	void setNodeBaseDemand(int nodeIndex, float newBaseDemand);
	
	double getNodePressure(int nodeIndex);

	double getNodeHead(int nodeIndex);
	
	// Pump Interfaces
	double getPumpCostPerDay(int linkIndex);
	double getPumpEnergyUsage(int linkIndex);

	void setPumpHeadFlowCurve(int linkIndex, int curveIndex);
	void setPumpEffeciencyFlowCurve(int linkIndex, int curveIndex);

	unsigned int getNumberOfPumpCurves();
	std::string getCurveID(int curveIndex);
	
private:
	bool isMetric();
};

#endif