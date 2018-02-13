#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H
#include "Texture.h"
#include "ExportHeader.h"
#include "MyTypeDefs.h"
#include "LinearNoise.h"

class ENGINE_SHARED NoiseTexture: public Texture
{
	LinearNoise linearNoise;
public:
	NoiseTexture(uint width, uint height, float gain, float lacunarity, float numOctaves);
	~NoiseTexture(void);

	virtual void bindTexture()const;
	virtual Texture* clone()const { return new NoiseTexture(*this); }
};

#endif