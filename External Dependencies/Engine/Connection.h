#ifndef CONNECTION_H
#define CONNECTION_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"

class ENGINE_SHARED Connection
{
	ushort endID;
	float length;

public:
	Connection(uint endID, float length);
	inline ushort getEndID() const {return endID;}
	inline float getLength() const {return length;} 
};

#endif