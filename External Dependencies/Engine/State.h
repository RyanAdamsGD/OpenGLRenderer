#ifndef STATE_H
#define STATE_H
#include "MyTypeDefs.h"

class State
{
public:
	uint type;
	virtual void update(float dt) = 0;
	State(uint type=0):type(type){}
};

#endif