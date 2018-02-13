#include "AutoTimer.h"

AutoTimer AutoTimer::timer;
void AutoTimer::start()
{
	QueryPerformanceCounter(&timer.begin);
}

float AutoTimer::end()
{
	QueryPerformanceFrequency(&timer.frequency);
	QueryPerformanceCounter(&timer.finish);
	if(timer.begin.QuadPart == 0)
	{
		QueryPerformanceCounter(&timer.begin);
	}

	return float(timer.finish.QuadPart - timer.begin.QuadPart)/float(timer.frequency.QuadPart);
}