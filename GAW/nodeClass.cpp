#include "StdAfx.h"
#include ".\nodeclass.h"

#include "EPAInterfaceClass.h"

nodeClass::nodeClass(void)
{
}

nodeClass::nodeClass(int index, std::string ID, float elevation, float baseDemand){
	this->setIndex(index);
	this->setID(ID);
	this->setElevation(elevation);
	this->setBaseDemand(baseDemand);
}

nodeClass::~nodeClass(void)
{
}

void nodeClass::setIndex(int newIndex){
	this->p_Index = newIndex;
}

int nodeClass::getIndex(){
	return this->p_Index;
}

void nodeClass::setID(std::string newID){
	this->p_ID = newID;
}

std::string nodeClass::getID(){
	return this->p_ID;
}

void nodeClass::setElevation(float newElevation){
	this->p_Elevation = newElevation;
}

float nodeClass::getElevation(){
	return this->p_Elevation;
}

void nodeClass::setBaseDemand(float newBaseDemand){
	this->p_BaseDemand = newBaseDemand;
}

float nodeClass::getBaseDemand(){
	return this->p_BaseDemand;
}

void nodeClass::setPressure(float newPressure){
	this->p_Pressure = newPressure;
}

float nodeClass::getPressure(){
	return this->p_Pressure;
}

void nodeClass::setHead(float newHead){
	this->p_Head = newHead;
}

float nodeClass::getHead(){
	return this->p_Head;
}

void nodeClass::updateEPANETCalculatedParameter(){
	EPAInterfaceClass EPAInterface;
	this->setPressure(EPAInterface.getNodePressure(this->getIndex()));
	this->setHead(EPAInterface.getNodeHead(this->getIndex()));
}

void nodeClass::updateEPANETDesignParameter(){
	EPAInterfaceClass EPAInterface;
	EPAInterface.setNodeElevation(this->getIndex(), this->getElevation());
	EPAInterface.setNodeBaseDemand(this->getIndex(), this->getBaseDemand());
}

void nodeClass::downloadEPANETDesignParamter(){
	EPAInterfaceClass EPAInterface;
	setID(EPAInterface.getNodeID(getIndex()));
	setElevation(EPAInterface.getNodeElevation(getIndex()));
	setBaseDemand(EPAInterface.getNodeBaseDemand(getIndex()));
}