#ifndef RAY_H
#define RAY_H
#include "glm\gtx\transform.hpp"
#include "ExportHeader.h"

class ENGINE_SHARED Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
public:
	Ray(const glm::vec3& origin, const glm::vec3& direction);
	///will return -1.0f if no intersection is found
	float intersetectsSphere(const glm::vec3& position, float radius);

	inline const glm::vec3& getOrigin() const {return origin;}
	inline const glm::vec3& getDirection() const {return direction;}
	inline void setOrigin(const glm::vec3& origin) {this->origin = origin;}
	inline void setDirection(const glm::vec3& direction) {this->direction = direction;}
};

#endif