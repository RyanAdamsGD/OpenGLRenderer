#include "Profiler.h"
#include "ExportHeader.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
using std::vector;
using std::string;
using std::endl;

#ifdef UPDATE_PROFILE
vector<profile*> update;
#endif
#ifdef DRAW_PROFILE
vector<profile*> draw;
#endif

profiler::profiler(const string& processName)
	{
		QueryPerformanceCounter(&start);
		QueryPerformanceFrequency(&frequency);
		process = processName;
		//_int64 test = *(_int64*)&start;
	}
profiler::~profiler()
	{
		QueryPerformanceCounter(&end);
		#ifdef UPDATE_PROFILE
		update.push_back(new profile(process,(((float)end.QuadPart - (float)start.QuadPart)/(float)frequency.QuadPart)));
		#endif
		#ifdef DRAW_PROFILE
		draw.push_back(new profile(process,(((float)end.QuadPart - (float)start.QuadPart)/(float)frequency.QuadPart)));
		#endif
	}

void __declspec( dllexport ) flushProfile()
{
	ofstream myfile;
	myfile.open("profiler.csv");
	int numberOfColumns =0;
	bool nameCheck = false;
#ifdef UPDATE_PROFILE
	for(unsigned int i=0;i<update.size();i++)
	{
		string check = update[0]->name;
		string check1;
		if(!nameCheck && i != 0)
		{
			while(check != check1)
			{
				check1 = update[i]->name;
				i++;
				numberOfColumns++;
			}
			nameCheck = true;
		}
	}
	for(unsigned int i=0;i<numberOfColumns;i++)
	{
		if(i+numberOfColumns<update.size()-1)
		{
			myfile<<update[i]->name<<", ";
		}
	}
	myfile<<endl;

	for(unsigned int i=0;i<update.size();i+=numberOfColumns)
	{
		if(i+numberOfColumns<update.size()-1)
		{
			for(int x=0;x<numberOfColumns;x++)
			{
				if(x-1 == numberOfColumns)
					myfile<<update[i+x]->data;
				else
					myfile<<update[i+x]->data <<", ";
			}
		}
		myfile<<endl;
	}

	for(unsigned int i =0;i<update.size();i++)
		free(update[i]);
#endif
#ifdef DRAW_PROFILE
		for(int i=0;i<draw.size();i++)
	{
		string check = draw[0]->name;
		string check1;
		if(!nameCheck && i != 0)
		{
			while(check != check1)
			{
				check1 = draw[i]->name;
				i++;
				numberOfColumns++;
			}
			nameCheck = true;
		}
	}
	for(unsigned int i=0;i<numberOfColumns;i++)
	{
		myfile<<draw[i]->name<<", ";
	}
	myfile<<endl;

	for(unsigned int i=0;i<draw.size();i++)
	{
		if(i+numberOfColumns<update.size()-1)
		{
			for(unsigned int x=0;x<numberOfColumns;x++)
			{
				if(x-1 == numberOfColumns)
					myfile<<draw[i+x]->data;
				else
					myfile<<draw[i+x]->data <<", ";

				i++;
			}
		}
		myfile<<endl;
	}
	for(unsigned int i = draw.size();i>=0;i++)
		free(draw[i]);
#endif

	myfile.close();
	
}