#ifndef AUTOTIMER_H
#define AUTOTIMER_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class __declspec( dllexport ) AutoTimer
{
	LARGE_INTEGER begin, finish, frequency;
	AutoTimer(){begin.QuadPart = 0; finish.QuadPart =0;}
	AutoTimer(const AutoTimer&){}
public:
	static AutoTimer timer;	
	static void start();
	static float end();
};

#endif