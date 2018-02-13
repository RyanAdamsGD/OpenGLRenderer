#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"
#include "Task.h"

class ENGINE_SHARED Scheduler
{
	float millisecondsPerFrame;
	uint currentFrame;
	static Scheduler instance;
	ulong frame;

	Scheduler(float millisecondsPerFrame);
	Scheduler(const Scheduler& value);
	~Scheduler(void);

	static const uint MAX_NUMBER_TASKS = 20;
	Task* tasks[MAX_NUMBER_TASKS];
	uint nextTask;
public:
	static void init(float millisecondsPerFrame);
	static void addTask(Task* task);
	static void run(float dt);
};

#endif