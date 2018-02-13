#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(const glm::vec3& position, const glm::vec3& diffuseColor, const glm::vec3& specularColor,float specularPower)
	:Light(position,diffuseColor,specularColor,specularPower,1)
{
}

DirectionalLight::DirectionalLight()
	:Light(glm::vec3(),glm::vec3(),glm::vec3(),0,1)
{
}

DirectionalLight::~DirectionalLight(void)
{
}

void DirectionalLight::updateDepthProjection()
{

}

void DirectionalLight::updateDepthView()
{

}

void DirectionalLight::setPosition(const glm::vec3& value)
{

}
