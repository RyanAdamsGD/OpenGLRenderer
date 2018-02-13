#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H
#include "Light.h"

class DirectionalLight : public Light
{
protected:
	virtual void updateDepthProjection();
	virtual void updateDepthView();
public:
	DirectionalLight();
	DirectionalLight(const glm::vec3& position, const glm::vec3& diffuseColor, const glm::vec3& specularColor,
		float specularPower);
	~DirectionalLight(void);

	virtual void setPosition(const glm::vec3& value);

};

#endif