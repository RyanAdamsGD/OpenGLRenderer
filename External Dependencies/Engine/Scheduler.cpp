#include "Scheduler.h"

#ifdef _DEBUG
Scheduler Scheduler::instance(0);
#endif

Scheduler::Scheduler(float millisecondsPerFrame)
	:millisecondsPerFrame(millisecondsPerFrame),nextTask(0)
{
}


Scheduler::~Scheduler(void)
{
}

Scheduler::Scheduler(const Scheduler& value)
{

}

void Scheduler::init(float millisecondsPerFrame)
{
	instance = Scheduler(millisecondsPerFrame);
}

void Scheduler::addTask(Task* task)
{
	instance.tasks[instance.nextTask++] = task;
}

void Scheduler::run(float dt)
{
	for(uint i=0;i<instance.nextTask;i++)
	{
		Task& task = *instance.tasks[i];
		if(task.getFrequency() % (task.getPhase() + instance.frame))
			task.process(dt);
	}
	instance.frame++;
}