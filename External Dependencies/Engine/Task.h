#ifndef TASK_H
#define TASK_H
#include "MyTypeDefs.h"
#include "FastDelegate.h"

class Task
{
	uint frequency;
	uint phase;
public:
	Task(uint frquency, uint phase) :frequency(frequency), phase(phase){}
	virtual void process(float dt) = 0;
	inline virtual uint getFrequency() {return frequency;}
	inline virtual uint getPhase() {return phase;}
};

#endif