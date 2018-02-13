#ifndef LIGHTING_MANAGER_H
#define LIGHTING_MANAGER_H
#include "Light.h"
#include "MyTypeDefs.h"
class DirectionalLight;

class LightingManager
{
	DirectionalLight* directionalLights;

	//pointers into one of the types of light arrays
	Light** lights;
	//indices into lights returned back to the user
	uint* indices;
	
	static LightingManager instance;

	LightingManager(void);
	LightingManager(const LightingManager&);
	LightingManager& operator =(const LightingManager&);
public:
	~LightingManager(void);

	uint addLight();
};

#endif