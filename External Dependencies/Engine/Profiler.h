#ifndef PROFILER
#define PROFILER
#include "ExportHeader.h"
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>


using namespace std;

struct __declspec( dllexport ) profile
{
	string name;
	float data;
public:
	profile(const std::string& n,float d)
	{
		name = n;
		data = d;
	}
	friend ENGINE_SHARED void flushProfile();
};

//turn on and off profiling
#define UPDATE_PROFILE
//#define DRAW_PROFILE



#ifdef UPDATE_PROFILE
//begin update profiling
#define PL(x) profiler(x);{
//end update profiling
#define EP	}
//begin draw profiling
#define DPL(x)
//end draw profile
#define DEP 
#elif DRAW_PROFILE
#define PL(x) 
#define EP	
#define DPL(x) profiler(x);{
#define DEP	}
#else
#define PL(x) 
#define EP	
#define DPL(x) 
#define DEP	
#endif

class __declspec( dllexport ) profiler
{
	LARGE_INTEGER start,end,frequency;
	string process;

public:

	profiler(const std::string& processName);
	~profiler();
};



#endif