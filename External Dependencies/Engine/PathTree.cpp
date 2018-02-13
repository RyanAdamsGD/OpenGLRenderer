#include "PathTree.h"


PathTree::PathTree(const PathingPoint& pathingPoint, uint numOfConnections)
	:pathingPoint(pathingPoint), numOfConnections(numOfConnections),connections(NULL)
{
	
}

void PathTree::addConnections(Connection* connections, uint numOfConnections)
{
	this->numOfConnections = numOfConnections;
	if(numOfConnections > 0)
	{
		uint t_size = sizeof(Connection) * numOfConnections;
		this->connections = (Connection*) malloc(t_size);
		memcpy(this->connections,connections,t_size);
	}
}

void PathTree::addConnections(Connection* connections)
{
	addConnections(connections,numOfConnections);
}


PathTree::~PathTree(void)
{
	free(connections);
}
