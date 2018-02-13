#ifndef PATHING_H
#define PATHING_H
#include "MyTypeDefs.h"
#include "ExportHeader.h"
#include "glm\gtx\transform.hpp"
#include "FastDelegate.h"
class PathTree;

class ENGINE_SHARED Pathing
{
	uint numPaths;
	PathTree* paths;
	struct Node
	{
		ushort parentID;
		ushort ID;
		float costSoFar;
		float estimatedTotalCost;
		Node(float costSoFar, float estimatedTotalCost, ushort ID, ushort parentID)
			:costSoFar(costSoFar), estimatedTotalCost(estimatedTotalCost), parentID(parentID),ID(ID)
		{}
	};
	
	Node* openList, *closedList;
	uint nextOpenListIndex, nextClosedListIndex;
	float currentLowestCost;

	static Pathing instance;
	Pathing();

	ushort getNearestNode(const glm::vec3& point);
	void resetLists();
	bool checkAgainstOpenList(const Node& node);
	bool checkAgainstClosedList(const Node& node);
	void removeNode(ushort nodeID, ushort parentID, Node* list, uint& nextNode);
	void checkNode(ushort pathID, float costSoFar, const glm::vec3& finalPointPosition);
	Node findLowestCostNode();
	glm::vec3* findPath(ushort startID, ushort endID, uint& numOfPathingPoints);
	glm::vec3* getReversePath(ushort startID, ushort endID, uint& numOfPathingPoints);
	float calculateDefaultHeuristic(glm::vec3 start, glm::vec3 end) {return 0;}
	static fastdelegate::FastDelegate2<glm::vec3,glm::vec3,float> defaultHeuristic;
	fastdelegate::FastDelegate2<glm::vec3,glm::vec3,float> calculateHeuristic;

public:
	static void addPath(PathTree* path, uint numPaths);
	static void cleanUp();
	static glm::vec3* getPath(const glm::vec3& currentPosition, const glm::vec3& endPosition, uint& numOfPathingPoints,fastdelegate::FastDelegate2<glm::vec3,glm::vec3,float> calculateHeuistic = defaultHeuristic);
};

#endif