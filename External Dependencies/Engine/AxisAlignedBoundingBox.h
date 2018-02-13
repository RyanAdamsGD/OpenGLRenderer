#ifndef AXIS_ALIGNED_BOUNDING_BOX_H
#define AXIS_ALIGNED_BOUNDING_BOX_H
#include <glm\gtx\transform.hpp>
class Ray;

class AxisAlignedBoundingBox
{
	glm::vec3 position;
	glm::vec3 scale;

	//returns an array to all of the points including the translation
	glm::vec3* getPoints()const;
public:
	///Build from just the scale and place at the origin
	AxisAlignedBoundingBox(const glm::vec3& scale);
	///Build from min and max points
	AxisAlignedBoundingBox(const glm::vec3& p1, const glm::vec3& p2);
	///Build from the points 
	AxisAlignedBoundingBox(const glm::vec3* const points, int count);
	~AxisAlignedBoundingBox(void);

	bool intersects(const glm::vec3& point)const;
	bool intersects(const AxisAlignedBoundingBox& AxisAlignedBoundingBox)const;
	bool intersects(const Ray& ray)const;

	inline void setPosition(const glm::vec3& value) { position = value; }
	inline void setScale(const glm::vec3& value) { scale = value; }

	AxisAlignedBoundingBox operator* (const glm::mat4& rhs);
};

#endif