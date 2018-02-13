#ifndef LINEARNOISE_H
#define LINEARNOISE_H
#include "LatticeNoise.h"
#include "ExportHeader.h"

class ENGINE_SHARED LinearNoise:public LatticeNoise
{
public:
	LinearNoise(void);
	LinearNoise(float gain, float lacunarity, const int numOctaves);

	virtual float valueNoise(const glm::vec3& point)const;
	virtual glm::vec3 vectorNoise(const glm::vec3& point)const;
};

#endif