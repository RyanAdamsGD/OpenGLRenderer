#include "AxisAlignedBoundingBox.h"
#include "MyTypeDefs.h"
#include "Helper Functions.h"

#define NUM_POINTS_IN_CUBE 8

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& scale)
	:position(0.0f)
{
	this->scale = glm::abs(scale);
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& p1, const glm::vec3& p2)
{
	scale = 0.5f * (p1 - p2);
	position = p2 + scale;
	scale = glm::abs(scale);
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3* const points, int count)
{
	glm::vec3 min(points[0]), max(points[0]);
	for(uint i=1;i<count;i++)
	{
		const glm::vec3& point = points[i];
		min.x = MIN(min.x,point.x);
		max.x = MAX(max.x,point.x);
		min.y = MIN(min.y,point.y);
		max.y = MAX(max.y,point.y);
		min.z = MIN(min.z,point.z);
		max.z = MAX(max.z,point.z);
	}

	scale = 0.5f * (max - min);
	position = min + scale;
	scale = glm::abs(scale);
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox(void)
{
}

bool AxisAlignedBoundingBox::intersects(const glm::vec3& point)const
{
	glm::vec3 offsetPoint = point - position;
	return abs(offsetPoint.x) <= abs(scale.x) && abs(offsetPoint.y) <= abs(scale.y) && abs(offsetPoint.z) <= abs(scale.z);
}

bool AxisAlignedBoundingBox::intersects(const AxisAlignedBoundingBox& axisAlignedBoundingBox)const
{
	glm::vec3* points = getPoints();
	for(uint i=0;i<NUM_POINTS_IN_CUBE;i++)
		if(axisAlignedBoundingBox.intersects(points[i]))
		{
			delete [] points;
			return true;
		}

	delete [] points;
	return false;
}

bool AxisAlignedBoundingBox::intersects(const Ray& ray)const
{
	//TODO:: implement this
	return false;
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::operator* (const glm::mat4& rhs)
{
	glm::vec3* points = getPoints();
	for(uint i=0;i<NUM_POINTS_IN_CUBE;i++)
	{
		points[i] -= position;
		points[i] = glm::vec3(glm::vec4(points[i],1.0f) * rhs);
		points[i] += position;
	}

	AxisAlignedBoundingBox boxToReturn(points,NUM_POINTS_IN_CUBE);
	delete [] points;
	return boxToReturn;
}

glm::vec3* AxisAlignedBoundingBox::getPoints()const
{
	glm::vec3 alteredScale(scale);
	glm::vec3* points =  new glm::vec3[NUM_POINTS_IN_CUBE];
	points[0] = position + alteredScale;
	points[1] = position - alteredScale;
	//flip the x
	alteredScale.x *= -1.0f;
	points[2] = position + alteredScale;
	points[3] = position - alteredScale;
	//flip the x and y which is the same as flipping the z
	alteredScale.y *= -1.0f;
	points[4] = position - alteredScale;
	points[5] = position + alteredScale;
	//flip the x back leaving only the y fliped
	alteredScale.x *= -1.0f;
	points[6] = position + alteredScale;
	points[7] = position - alteredScale;

	return points;
}
