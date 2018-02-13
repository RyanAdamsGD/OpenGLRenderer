#include "LightingManager.h"
#include "LuaState.h"
#include "DirectionalLight.h"


LightingManager LightingManager::instance;

LightingManager::LightingManager(void)
{
	LuaState luaState("../../External Dependencies/Engine/config.lua");
	uint maxNumLights = luaState.get<int>("MAX_NUM_LIGHTS");
	lights = new Light*[maxNumLights];
	indices = new uint[maxNumLights];

	directionalLights = new DirectionalLight[maxNumLights];
}


LightingManager::~LightingManager(void)
{
}
