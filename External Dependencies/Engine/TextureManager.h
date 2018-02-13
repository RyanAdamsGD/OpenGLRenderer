#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include "ExportHeader.h"
#include <vector>
#include "MyTypeDefs.h"
#include <glm\gtx\transform.hpp>
class Texture;

class ENGINE_SHARED TextureManager
{
	//getTexture will be called often enough that this is worth storing
	uint textureCount;
	std::vector<Texture*> textures;

	static TextureManager instance;
	TextureManager(void);
	TextureManager(const TextureManager&);
	TextureManager& operator =(const TextureManager&);
public:
	~TextureManager(void);
	static TextureManager& getInstance() { return instance; }

	uint addFileTexture(const char* fileName);
	uint addSolidColorTexture(const glm::vec4& color);
	uint addNoiseTexture(uint width = 128, uint height = 128, float gain = 0.5f, float lacunarity = 2.0f, float numOctaves = 2.0f);

	Texture* getTexture(uint index);
	const uint& getTotalNumberOfTextures()const { return textureCount; }
};

#define Texture_Manager TextureManager::getInstance()

#endif