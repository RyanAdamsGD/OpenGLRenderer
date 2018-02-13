#ifndef TRANSFORM_INFO_H
#define TRANSFORM_INFO_H
#include <glm\core\type_vec.hpp>
#include <glm\gtx\transform.hpp>
#include "ExportHeader.h"

class ENGINE_SHARED TransformInfo
{
	glm::mat4 transform;
	//rotations in degrees
	glm::vec3 rotationXYZ;
	glm::vec3 scaleXYZ;
	glm::vec3 position;
	void updateMatrix();
public:
	TransformInfo(void);

	float getTranslationX() const { return position.x; }
	float getTranslationY() const { return position.y; }
	float getTranslationZ() const { return position.z; }
	float getRotationX() const {return rotationXYZ.x; }
	float getRotationY() const {return rotationXYZ.y; }
	float getRotationZ() const {return rotationXYZ.z; }
	float getScaleX() const { return scaleXYZ.x; }
	float getScaleY() const { return scaleXYZ.y; }
	float getScaleZ() const { return scaleXYZ.z; }
	const glm::vec3& getTranslation() const { return position; }
	const glm::mat4& getTransform() const {return transform; }

	void setRotationX(float angle);
	void setRotationY(float angle);
	void setRotationZ(float angle);
	void setPosition(const glm::vec3& position);
	void setTranslationX(float translation);
	void setTranslationY(float translation);
	void setTranslationZ(float translation);
	void setScaleX(float scale);
	void setScaleY(float scale);
	void setScaleZ(float scale);

	//TODO:: adjust the other values when this is called
	void setMatrix(const glm::mat4& matrix) { transform = matrix; }
};

#endif