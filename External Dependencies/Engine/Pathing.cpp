#include "Pathing.h"
#include "PathTree.h"

#ifdef _DEBUG
Pathing Pathing::instance;
fastdelegate::FastDelegate2<glm::vec3,glm::vec3,float> Pathing::defaultHeuristic = fastdelegate::MakeDelegate(&Pathing::instance,&Pathing::calculateDefaultHeuristic);
#endif

Pathing::Pathing()
	:paths(NULL),numPaths(0),nextOpenListIndex(0),nextClosedListIndex(0),currentLowestCost(FLOAT_MAX)
{
}

void Pathing::addPath(PathTree* path, uint numPaths)
{
	uint sizeOfCurrentPaths = instance.numPaths * sizeof(PathTree);
	instance.numPaths += numPaths;
	PathTree* pathHolder = instance.paths;
	if(instance.paths)
	{
		free(instance.paths);
	}
	instance.paths = (PathTree*) malloc(instance.numPaths * sizeof(PathTree));

	memcpy(instance.paths,pathHolder,sizeOfCurrentPaths);
	memcpy(instance.paths + sizeOfCurrentPaths,path,instance.numPaths * sizeof(PathTree) - sizeOfCurrentPaths);
}

void Pathing::cleanUp()
{
	free(instance.paths);
}

ushort Pathing::getNearestNode(const glm::vec3& point)
{
	ushort nearestNodeID = 0;
	float shortestDist = FLOAT_MAX;
	for(uint i=0;i<instance.numPaths;i++)
	{
		//hopefully would do ray cast here to check you wont walk into a wall to get to the nearest point
		float length = glm::length(instance.paths[i].pathingPoint.getPosition() - point);
		if(length < shortestDist)
		{
			shortestDist = length;
			nearestNodeID = instance.paths[i].pathingPoint.getID();
		}
	}
	return nearestNodeID;
}


glm::vec3* Pathing::getPath(const glm::vec3& currentPosition, const glm::vec3& endPosition, uint& numOfPathingPoints,fastdelegate::FastDelegate2<glm::vec3,glm::vec3,float> calculateHeuistic)
{
	instance.calculateHeuristic = calculateHeuistic;
	//we get our 2 pathing nodes
	ushort startNodeID = instance.getNearestNode(currentPosition);
	ushort endNodeID = instance.getNearestNode(endPosition);
	instance.currentLowestCost = FLOAT_MAX;
	glm::vec3* path;
	glm::vec3* pathHolder;
	//check if nodes take us out of the way
	if(glm::length(currentPosition - endPosition) > glm::length(currentPosition - instance.paths[startNodeID].pathingPoint.getPosition()))
	{
		path = instance.findPath(startNodeID, endNodeID, numOfPathingPoints);

		//add in end position
		numOfPathingPoints++;
		pathHolder = (glm::vec3*) malloc(sizeof(glm::vec3) * numOfPathingPoints);
		memcpy(pathHolder,path,sizeof(glm::vec3) * (numOfPathingPoints-1));
		pathHolder[numOfPathingPoints-1] = endPosition;
		free(path);
	}
	else
	{
		pathHolder = (glm::vec3*) malloc(sizeof(glm::vec3));
		pathHolder[0] = endPosition;
		numOfPathingPoints = 1;
	}

	free(instance.openList);
	instance.openList = NULL;
	free(instance.closedList);
	instance.closedList = NULL;
	return pathHolder;
}

void Pathing::resetLists()
{
	if(instance.openList)
	{
		free(instance.openList);
	}
	if(instance.closedList)
	{
		free(instance.closedList);
	}
	nextOpenListIndex=0;
	nextClosedListIndex=0;
	openList = (Node*) malloc(sizeof(Node) * numPaths);
	closedList = (Node*) malloc(sizeof(Node) * numPaths);
}

void Pathing::checkNode(ushort pathID, float costSoFar, const glm::vec3& finalPointPosition)
{
	PathTree& path = paths[pathID];
	Node* nodes = (Node*) malloc(sizeof(Node) * path.numOfConnections);

	for(uint i=0;i<path.numOfConnections;i++)
	{		
		//could thread this functionality
		float costToThisNode = costSoFar + path.connections[i].getLength();
		float heuristic = glm::length(path.pathingPoint.getPosition() - finalPointPosition) 
			+ calculateHeuristic(path.pathingPoint.getPosition(),paths[path.connections[i].getEndID()].pathingPoint.getPosition());
		nodes[i] = Node(costToThisNode,costToThisNode + heuristic,path.connections[i].getEndID(),path.pathingPoint.getID());
		if(!checkAgainstClosedList(nodes[i]))
			checkAgainstOpenList(nodes[i]);
	}

	free(nodes);
}

bool Pathing::checkAgainstClosedList(const Node& node)
{
	for(uint i=0;i<nextClosedListIndex;i++)
	{
		if(node.ID == closedList[i].ID)
		{
			if(node.estimatedTotalCost < closedList[i].estimatedTotalCost && node.costSoFar < closedList[i].costSoFar)
			{
				openList[nextOpenListIndex++] = node;
				removeNode(closedList[i].ID,closedList[i].parentID,closedList,nextClosedListIndex);
			}
			return true;
		}
	}
	return false;
}

void Pathing::removeNode(ushort nodeID, ushort parentID, Node* list, uint& nextNode)
{
	for(uint i=0;i<nextNode;i++)
	{
		if(nodeID == list[i].ID && parentID == list[i].parentID)
		{
			memcpy(&list[i],&list[i+1], (nextNode - (i +1)) *sizeof(Node));
			nextNode--;
			break;
		}
	}
}


bool Pathing::checkAgainstOpenList(const Node& node)
{
	for(uint j=0;j<nextOpenListIndex;j++)
	{
		if(openList[j].ID == node.ID)
		{
			//node is already in list don't add it again
			if(openList[j].costSoFar > node.costSoFar)
			{
				openList[j] = node;
			}
			return true;
		}
	}
	openList[nextOpenListIndex++] = node;
	return false;
}


glm::vec3* Pathing::findPath(ushort startID, ushort endID, uint& numOfPathingPoints)
{
	resetLists();
	glm::vec3 finalPosition = paths[endID].pathingPoint.getPosition();
	openList[nextOpenListIndex++] = Node(0,glm::length(finalPosition - paths[startID].pathingPoint.getPosition()),startID,startID);
	bool atFinalNode = false;

	while(nextOpenListIndex > 0 && !atFinalNode)
	{
		Node node = findLowestCostNode();
		if(node.ID == endID)
			atFinalNode = true;
		else
			checkNode(node.ID,node.costSoFar,finalPosition);

		removeNode(node.ID,node.parentID,openList,nextOpenListIndex);
		if(!checkAgainstClosedList(node))
			closedList[nextClosedListIndex++] = node;
	}

	return getReversePath(startID, endID,numOfPathingPoints);
}


glm::vec3* Pathing::getReversePath(ushort startID ,ushort endID, uint& numOfPathingPoints)
{
	uint count = 0;
	Node node = closedList[0];
	glm::vec3* path = (glm::vec3*) malloc(sizeof(glm::vec3) * numPaths);

	for(uint i=0;i<nextClosedListIndex;i++)
	{
		if(closedList[i].ID == endID)
		{
			node = closedList[i];
			path[count++] = paths[node.ID].pathingPoint.getPosition();
			break;
		}
	}

	do
	{
		for(uint i=0;i<nextClosedListIndex;i++)
		{
			if(node.parentID == closedList[i].ID)
			{
				node = closedList[i];
				path[count++] = paths[node.ID].pathingPoint.getPosition();
				break;
			}
		}
	}while(node.parentID != startID);

	for(uint i=0;i<nextClosedListIndex;i++)
	{
		if(closedList[i].ID == startID)
		{
			node = closedList[i];
			path[count++] = paths[node.ID].pathingPoint.getPosition();
			break;
		}
	}

	glm::vec3* reversedPaths = (glm::vec3*) malloc(sizeof(glm::vec3) * count);
	for(uint i=0;i<count;i++)
	{
		reversedPaths[i] = path[count-1-i];
	}
	free(path);
	numOfPathingPoints = count;
	return reversedPaths;
}

Pathing::Node Pathing::findLowestCostNode()
{
	uint lowestNodeIndex = 0;
	float cost = FLOAT_MAX;

	for(uint i=0;i<nextOpenListIndex;i++)
	{
		Node& node = openList[i];
		if(node.estimatedTotalCost < cost)
		{
			cost = node.estimatedTotalCost;
			lowestNodeIndex = i;
		}
	}
	return openList[lowestNodeIndex];
}