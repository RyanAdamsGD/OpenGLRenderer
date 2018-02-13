#include "TransformInfo.h"
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>

TransformInfo::TransformInfo(void)
	:transform(),rotationXYZ(0.0f,0.0f,0.0f),scaleXYZ(1.0f,1.0f,1.0f)
{
}

void TransformInfo::setScaleX(float scale)
{
	scaleXYZ.x = scale;
	updateMatrix();
}

void TransformInfo::setScaleY(float scale)
{
	scaleXYZ.y = scale;
	updateMatrix();
}

void TransformInfo::setScaleZ(float scale)
{
	scaleXYZ.z = scale;
	updateMatrix();
}

void TransformInfo::setRotationX(float angle)
{
	rotationXYZ.x = angle;
	updateMatrix();
}

void TransformInfo::setRotationY(float angle)
{
	rotationXYZ.y = angle;
	updateMatrix();
}

void TransformInfo::setRotationZ(float angle)
{
	rotationXYZ.z = angle;
	updateMatrix();
}

void TransformInfo::updateMatrix()
{
	glm::quat xrot = glm::angleAxis(rotationXYZ.x,glm::vec3(1.0f,0.0f,0.0f));
	glm::quat yrot = glm::angleAxis(rotationXYZ.y,glm::vec3(0.0f,1.0f,0.0f));
	glm::quat zrot = glm::angleAxis(rotationXYZ.z,glm::vec3(0.0f,0.0f,1.0f));

	glm::mat4 totalRotation = glm::toMat4(xrot * yrot * zrot);

	transform = glm::translate(getTranslation()) * totalRotation * glm::scale(scaleXYZ);
}

void TransformInfo::setPosition(const glm::vec3& position)
{
	this->position = position;
	transform[0][3] = position.x;
	transform[1][3] = position.y;
	transform[2][3] = position.z;
}

void TransformInfo::setTranslationX(float translation)
{ 
	position.x = translation;
	transform[0][3] = translation;
}

void TransformInfo::setTranslationY(float translation)
{ 
	position.y = translation;
	transform[1][3] = translation;
}
void TransformInfo::setTranslationZ(float translation)
{
	position.z = translation;
	transform[2][3] = translation;
}