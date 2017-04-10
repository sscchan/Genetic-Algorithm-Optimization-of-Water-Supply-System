#include "StdAfx.h"
#include ".\pipeclass.h"
#include "EPAInterfaceClass.h"
#include "pipeDatabaseClass.h"

pipeClass::pipeClass(void)
{
}

pipeClass::pipeClass(int index, std::string ID, unsigned int pipeType, float length, float roughness){
	this->setIndex(index);
	this->setID(ID);
	this->setPipeType(pipeType);
	this->setLength(length);
	this->setRoughness(roughness);
}
pipeClass::~pipeClass(void)
{
}

void pipeClass::setIndex(int newIndex){
	this->p_Index = newIndex;
}

int pipeClass::getIndex(){
	return this->p_Index;
}
void pipeClass::setID(std::string newID){
	this->p_ID = newID;
}

std::string pipeClass::getID(){
	return this->p_ID;
}

void pipeClass::setPipeType(unsigned int newPipeType){
	this->p_PipeType = newPipeType;
	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::
		Instance();
	setDiameter(pipeDatabase->getdiameter(p_PipeType));
	setRoughness(pipeDatabase->getRoughness(p_PipeType));
}

unsigned int pipeClass::getPipeType(){
	return this->p_PipeType;
}

void pipeClass::setDiameter(float newDiameter){
	this->p_Diameter = newDiameter;
}

float pipeClass::getDiameter(){
	return this->p_Diameter;
}

void pipeClass::setLength(float newLength){
	this->p_Length = newLength;
}

float pipeClass::getLength(){
	return this->p_Length;
}

void pipeClass::setRoughness(float newRoughness){
	this->p_Roughness = newRoughness;
}

float pipeClass::getRoughness(){
	return this->p_Roughness;
}

void pipeClass::setFlow(float newFlow){
	this->p_Flow = newFlow;
}

float pipeClass::getFlow(){
	return this->p_Flow;
}

void pipeClass::setVelocity(float newVelocity){
	this->p_Velocity = newVelocity;
}

float pipeClass::getVelocity(){
	return this->p_Velocity;
}

void pipeClass::setHeadloss(float newHeadLoss){
	this->p_Headloss = newHeadLoss;
}

float pipeClass::getHeadloss(){
	return this->p_Headloss;
}

float pipeClass::getMonetaryCost(){
	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::Instance();
	
	return pipeDatabase->getUnitLengthMonetaryCost(p_PipeType) * getLength();
}

float pipeClass::getCarbonCost(){
	pipeDatabaseClass* pipeDatabase = pipeDatabaseClass::Instance();

	return pipeDatabase->getUnitLengthCarbonCost(p_PipeType) * p_Length;
}
void pipeClass::updateEPANETDesignParameter(){
	EPAInterfaceClass EPAInterface;
	EPAInterface.setLinkDiameter(this->getIndex(), this->getDiameter());
	EPAInterface.setLinkRoughness(this->getIndex(), this->getRoughness());
}

void pipeClass::updateEPANETCalculatedParameter(){
	// flow velocity and headloss
	EPAInterfaceClass EPAInterface;
	this->setFlow(EPAInterface.getLinkFlow(this->getIndex()));
	this->setVelocity(EPAInterface.getLinkVelocity(this->getIndex()));
	this->setHeadloss(EPAInterface.getLinkHeadloss(this->getIndex()));
}

void pipeClass::downloadEPANETDesignParameter(){	
	EPAInterfaceClass EPAInterface;
	p_ID = EPAInterface.getLinkID(this->getIndex());
	p_Length = EPAInterface.getLinkLength(this->getIndex());
	p_Diameter = EPAInterface.getLinkDiameter(getIndex());
	p_Roughness = EPAInterface.getLinkRoughness(getIndex());
}