#ifndef PATHINGPOINT_H
#define PATHINGPOINT_H
#include "glm\gtx\transform.hpp"
#include "MyTypeDefs.h"
#include "ExportHeader.h"

class ENGINE_SHARED PathingPoint
{
	ushort ID;
	glm::vec3 position;
public:
	PathingPoint(const glm::vec3& position, ushort ID);
	inline const glm::vec3& getPosition() const {return position;}
	inline ushort getID() {return ID;}
};

#endif