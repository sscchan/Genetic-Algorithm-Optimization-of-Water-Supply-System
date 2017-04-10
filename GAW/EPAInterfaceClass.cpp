#include "StdAfx.h"
#include "assert.h"
#include ".\epainterfaceclass.h"
#include "epanet2.h"


EPAInterfaceClass::EPAInterfaceClass(void)
{
}

EPAInterfaceClass::~EPAInterfaceClass(void)
{
}

/** 
 * a function determining the whether the Metric units are used in EPANET input.
 * @return whether metric units are used
 */
bool EPAInterfaceClass::isMetric(){
	static bool isMetricUnit_;
	int unitCode;	// a code returned by EPANET corresponding to the unit being used
	
	int errorCode;
	errorCode = ENgetflowunits (&unitCode);
	assert (errorCode == 0);
	
	// EPANET returns code 0->4 for US units, and 5->10 for metric units
	if (unitCode <= 4)
		isMetricUnit_ = false;
	else
		isMetricUnit_ = true;
	
	return isMetricUnit_;
}


int EPAInterfaceClass::getLinkType(int linkIndex){
	int linkType;
	
	int errorCode;
	errorCode = ENgetlinktype(linkIndex, &linkType);

	assert(errorCode == 0);

	return linkType;
}
/**
 * a function retriving the flow of a particular link.
 * Retrieves the flow of a particular link, converting them to m^3/s.
 * @param linkIndex the index of the link of which the flow is required.
 * @return the flow of the link.
 */
double EPAInterfaceClass::getLinkFlow(int linkIndex){
	float linkFlow_;
	
	int errorCode;
	errorCode = ENgetlinkvalue(linkIndex, EN_FLOW, &linkFlow_);
	
	assert(errorCode == 0);

	return linkFlow_;
}

/** 
 * a function returning the number of links in the network.
 * @return the number of links.
 */
int EPAInterfaceClass::getNumberOfLinks(void){
	int numberOfLinks_;
	(ENgetcount(EN_LINKCOUNT, &numberOfLinks_));
	return numberOfLinks_;
}

/**
 * a function that returns the index of the starting node of a given link.
 * @param linkIndex the index of the link.
 * @return the index of the starting node.
 */
int EPAInterfaceClass::getLinkStartNode(int linkIndex){
	int dummyEndNode_;
	int startingNode_;
	(ENgetlinknodes(linkIndex, &startingNode_, &dummyEndNode_));
	return startingNode_;
}

/**
 * a function that returns the index of the ending node of a given link.
 * @param linkIndex the index of the link.
 * @return the index of the ending node.
 */
int EPAInterfaceClass::getLinkEndNode(int linkIndex){
	int dummyStartNode_;
	int endingNode_;
	(ENgetlinknodes(linkIndex, &dummyStartNode_, &endingNode_));
	return endingNode_;
}

/**
 * a function that returns the ID of a particular link.
 * @param linkIndex index of the link.
 * @return the ID of the link
 */
std::string EPAInterfaceClass::getLinkID(int linkIndex){
	std::string linkID_;
	char bufferID[40];
	ENgetlinkid(linkIndex, bufferID);

	linkID_ = bufferID;
	return linkID_;
}

/** 
 * a function that returns a particular link's diameter.
 * @param linkIndex index of the link.
 * @return the diameter of the link, in metres.
 */
double EPAInterfaceClass::getLinkDiameter(int linkIndex){
	float linkDiameter_;

	int errorCode;
	errorCode = (ENgetlinkvalue(linkIndex, EN_DIAMETER, &linkDiameter_));
	
	assert(errorCode == 0);

	return linkDiameter_;
}

void EPAInterfaceClass::setLinkDiameter(int linkIndex, float newDiameter){
	ENsetlinkvalue(linkIndex, EN_DIAMETER, newDiameter);
}
/**
 * a function that returns the length of a link in meters
 * @param linkIndex index of the link.
 * @return the length of the link, in metres.
 */
double EPAInterfaceClass::getLinkLength(int linkIndex){
	float linkLength_;
	
	int errorCode;
	errorCode = (ENgetlinkvalue(linkIndex, EN_LENGTH, &linkLength_));

	assert(errorCode == 0);

	return linkLength_;
}

/**
 * a function that returns the roughness of a link, in metres.
 * @param linkIndex the index of the link.
 * @return the roughness, in metres.
 */
double EPAInterfaceClass::getLinkRoughness(int linkIndex){
	float linkRoughness_;
	int errorCode;
	errorCode = (ENgetlinkvalue(linkIndex, EN_ROUGHNESS, &linkRoughness_));
	
	assert(errorCode == 0);
	return linkRoughness_;
}

void EPAInterfaceClass::setLinkRoughness(int linkIndex, float newRoughness){
	int errorCode;
	errorCode = ENsetlinkvalue(linkIndex, EN_ROUGHNESS, newRoughness);

	assert(errorCode == 0);
}

/**
 * a function that retrieve the velocity of a particular link.
 * @param linkIndex the index of the link.
 * @return the veloicty, in m/s
 */
double EPAInterfaceClass::getLinkVelocity(int linkIndex){
	float linkVelocity_;
	int errorCode;
	errorCode = (ENgetlinkvalue(linkIndex, EN_VELOCITY, &linkVelocity_));
	
	assert(errorCode == 0);
	return linkVelocity_;
}

double EPAInterfaceClass::getLinkHeadloss(int linkIndex){
	float linkHeadloss_;
	int errorCode;
	errorCode = ENgetlinkvalue(linkIndex, EN_HEADLOSS, &linkHeadloss_);

	assert (errorCode == 0);
	return linkHeadloss_;
}
// Node Interfaces
/** 
 * a function that retruns the number of nodes in the network file.
 * @return number of nodes.
 */
int EPAInterfaceClass::getNumberOfNodes(void){
	int numberOfNodes_;

	int errorCode;
	errorCode = (ENgetcount(EN_NODECOUNT, &numberOfNodes_));
	assert(errorCode == 0);

	return numberOfNodes_;
}

int EPAInterfaceClass::getNodeType(int nodeIndex){
	int nodeType;

	int errorCode;
	errorCode = ENgetnodetype(nodeIndex, &nodeType);
	assert(errorCode == 0);

	return nodeType;
}
/**
 * a function test whether a node is a reservior or not.
 * @param nodeIndex the index of the node.
 * @return the true / false test result
 */
bool EPAInterfaceClass::isNodeReservior(int nodeIndex){
	int nodeType_;

	int errorCode;
	errorCode = (ENgetnodetype(nodeIndex, &nodeType_));

	assert(errorCode == 0);

	if (nodeType_ == 1)
		return true;
	else
		return false;
}

/** 
 * a function that returns the ID of a particular node.
 * @param nodeIndex the index of the node.
 * @return the ID of the node
 */
std::string EPAInterfaceClass::getNodeID(int nodeIndex){
	std::string nodeID_;
	char nodeBuffer_[15];
	(ENgetnodeid(nodeIndex, nodeBuffer_));
	nodeID_ = nodeBuffer_;
	return nodeID_;
}

/**
 * a function that returns the elevation of a particular node in meters.
 * @param nodeIndex the index of the node.
 * @return the elevation of the node in meters
 */
double EPAInterfaceClass::getNodeElevation(int nodeIndex){
	float nodeElevation_;

	int errorCode;
	errorCode = ENgetnodevalue(nodeIndex, EN_ELEVATION, &nodeElevation_);
	
	assert(errorCode == 0);
	return nodeElevation_;
}
void EPAInterfaceClass::setNodeElevation(int nodeIndex, float newElevation){
	int errorCode;
	errorCode = ENsetnodevalue(nodeIndex,EN_ELEVATION, newElevation);

	assert(errorCode == 0);
}

/**
 * a function that returns the demand of a particular node.
 * @param nodeIndex the index of the node.
 * @return the demand of the node in m^3/s
 */

double EPAInterfaceClass::getNodeBaseDemand(int nodeIndex){

	float nodeBaseDemand_;
	
	int errorCode;
	errorCode =  (ENgetnodevalue(nodeIndex, EN_BASEDEMAND, &nodeBaseDemand_));

	assert(errorCode == 0);

	return nodeBaseDemand_;
}

void EPAInterfaceClass::setNodeBaseDemand(int nodeIndex, float newBaseDemand){
	int errorCode;
	
	errorCode = ENsetnodevalue(nodeIndex, EN_BASEDEMAND, newBaseDemand);

	assert(errorCode == 0);
}
/**
 * a function that retrieves the pressure head at a particular node
 * @param nodeIndex index of the node
 * @return the pressure of the node, in metres
 */
double EPAInterfaceClass::getNodePressure(int nodeIndex){
	float nodePressure_;

	int errorCode;
	errorCode = (ENgetnodevalue(nodeIndex,EN_PRESSURE, &nodePressure_));
	
	assert(errorCode == 0);
	
	//TODO: Find out why the pressure is *10 meter...
	nodePressure_ = nodePressure_ / 10;
	return nodePressure_;
}

double EPAInterfaceClass::getNodeHead(int nodeIndex){
	float nodeHead_;

	int errorCode;
	errorCode = ENgetnodevalue(nodeIndex, EN_HEAD, &nodeHead_);

	assert(errorCode == 0);

	return nodeHead_;
}

double EPAInterfaceClass::getPumpCostPerDay(int linkIndex){
	return ENgetPumpCostPerDay(linkIndex);
}

double EPAInterfaceClass::getPumpEnergyUsage(int linkIndex){
	return ENgetPumpEnergyUsage(linkIndex);
}

void EPAInterfaceClass::setPumpHeadFlowCurve(int linkIndex, int curveIndex){
	ENsetPumpHeadFlowCurve(linkIndex, curveIndex);
}

void EPAInterfaceClass::setPumpEffeciencyFlowCurve(int linkIndex, int curveIndex){
	ENsetPumpEffeciencyFlowCurve(linkIndex, curveIndex);
}

unsigned int EPAInterfaceClass::getNumberOfPumpCurves()
{
	return ENgetNumberOfCurves();
}

std::string EPAInterfaceClass::getCurveID(int curveIndex)
{
	char bufferChar[40];

	std::string _curveID;
	ENgetCurveID(curveIndex, bufferChar);
	_curveID = bufferChar;
	return _curveID;
}