#ifndef PUMPTYPECLASS_H
#define PUMPTYPECLASS_H
#pragma once

class pumpTypeClass
{
public:
	pumpTypeClass(void);
	~pumpTypeClass(void);
	
	std::string pumpName;
	int headFlowCurve;
	int effeciencyFlowCurve;
};

#endif