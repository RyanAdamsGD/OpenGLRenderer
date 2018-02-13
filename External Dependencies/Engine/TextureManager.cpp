#include "TextureManager.h"
#include "Logger.h"
#include "FileTexture.h"
#include "SolidColorTexture.h"
#include "NoiseTexture.h"

TextureManager TextureManager::instance;

TextureManager::TextureManager(void)
{
}

TextureManager::~TextureManager(void)
{
	for(uint i=0;i<textureCount;i++)
	{
		if(textures[i] != NULL)
		{
			delete textures[i];
			textures[i] = NULL;
		}
	}
	textures.clear();
	textureCount = 0;
}

Texture* TextureManager::getTexture(uint index)
{
	if(index > textureCount)
	{
#ifdef USE_LOGGER
		char message[70];
		sprintf(message,"Requested texture index: %u is out of range.", index);
		LOGW(message);
#endif
		return NULL;
	}

	return textures[index];
}

uint TextureManager::addFileTexture(const char* fileName)
{
	textures.push_back(new FileTexture(fileName));
	textureCount = textures.size();
	return textureCount - 1;
}

uint TextureManager::addSolidColorTexture(const glm::vec4& color)
{
	textures.push_back(new SolidColorTexture(color));
	textureCount = textures.size();
	return textureCount - 1;
}

uint TextureManager::addNoiseTexture(uint width, uint height, float gain, float lacunarity, float numOctaves)
{
	textures.push_back(new NoiseTexture(width,height,gain,lacunarity,numOctaves));
	textureCount = textures.size();
	return textureCount - 1;
}