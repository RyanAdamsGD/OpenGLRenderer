#include "Ray.h"


Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
	:origin(origin), direction(direction)
{
}

float Ray::intersetectsSphere(const glm::vec3& position, float radius)
{
	glm::vec3 Q = position - origin;
	float c = glm::length(Q);
	float v = glm::dot(Q,direction);
	float d = radius * radius - (c*c - v*v);

	if(d <0.0f)
		return -1.0f;

	return (v-sqrtf(d));
}