#ifndef CUBICNOISE_H
#define CUBICNOISE_H
#include "LatticeNoise.h"
#include "ExportHeader.h"

class ENGINE_SHARED CubicNoise:public LatticeNoise
{
public:
	CubicNoise(void);
	CubicNoise(float gain, float lacunarity, const int numOctaves);

	virtual float valueNoise(const glm::vec3& point)const;
	virtual glm::vec3 vectorNoise(const glm::vec3& point)const;
};

#endif