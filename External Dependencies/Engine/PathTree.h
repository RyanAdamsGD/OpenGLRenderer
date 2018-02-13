#ifndef PATHTREE_H
#define PATHTREE_H
#include "ExportHeader.h"
#include "PathingPoint.h"
#include "Connection.h"

class ENGINE_SHARED PathTree
{
public:
	uint numOfConnections;
	PathingPoint pathingPoint;
	Connection* connections;

	PathTree(const PathingPoint& pathingPoint, uint numOfConnections);
	void addConnections(Connection* connections, uint numOfConnections);
	void addConnections(Connection* connections);
	~PathTree(void);
};

#endif