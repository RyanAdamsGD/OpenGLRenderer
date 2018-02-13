#ifndef LIGHT_H
#define LIGHT_H
#include "MyTypeDefs.h"
#include <glm\gtx\transform.hpp>

class Light
{
protected:
	glm::vec3 position;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularPower;
	//number of shadow textures that need to be generated
	uint perspectiveCount;
	glm::mat4 depthProjection;
	glm::mat4 depthView;

	virtual void updateDepthProjection() = 0;
	virtual void updateDepthView() = 0;
public:
	Light(const glm::vec3& position, const glm::vec3& diffuseColor, const glm::vec3& specularColor,
		float specularPower,uint perspectiveCount)
		:position(position),diffuseColor(diffuseColor),specularColor(specularColor),specularPower(specularPower),
		perspectiveCount(perspectiveCount)
	{}

	inline void setDiffuseColor(const glm::vec3& color) { diffuseColor = color; }
	inline void setSpecularColor(const glm::vec3& color) { specularColor = color; }
	inline void setColor(const glm::vec3& color) { diffuseColor = color; specularColor = color; }
	inline void setSpecularPower(float value) { specularPower = value; }

	virtual void setPosition(const glm::vec3& value) = 0;

	inline const glm::vec3& getDiffuseColor() const { return diffuseColor; }
	inline const glm::vec3& getSpecularColor() const { return specularColor; }
	inline const glm::vec3& getPosition() const { return position; }
	inline const float& getSpecularPower() const { return specularPower; }
};

#endif