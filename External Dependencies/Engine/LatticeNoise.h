#ifndef LATTICENOISE_H
#define LATTICENOISE_H
#include <glm\gtx\transform.hpp>
#include "ExportHeader.h"
//these are used in noise functions
//because of the recursive perm calls in index it doesn't always inline
//properly so they are used a macros
#define PERM(x) permutationTable[(x)&kTableMask]
#define INDEX(ix,iy,iz) PERM((ix) + PERM((iy) + PERM(iz)))
#define FLOOR(x) ((int)(x) - ((x) < 0 && (x) != (int) (x)))


class ENGINE_SHARED LatticeNoise
{
	int numOctaves;			//best to keep this between 1-8 for most scenes

	//brownian motion
	float gain, lacunarity, fbmMin, fbmMax;

	void initValueTable(int seed);
	void initVectorTable(int seed);
	void computeFbmBounds();
public:
	static const int kTableSize = 256;
	static const int kTableMask = kTableSize - 1;
	static const unsigned int seedValue = 253;
protected:
	static const unsigned char permutationTable[kTableSize];
	float valueTable[kTableSize];
	glm::vec3 vectorTable[kTableSize];
public:
	LatticeNoise(void);
	LatticeNoise(float gain, float lacunarity, const int numOctaves);

	float valueFBM(const glm::vec3& point)const;
	glm::vec3 vectorFBM(const glm::vec3& point)const;

	float valueTurbulence(const glm::vec3& point)const;

	virtual float valueNoise(const glm::vec3& point)const = 0;
	virtual glm::vec3 vectorNoise(const glm::vec3& point)const = 0;

	void setNumOctaves(int value);
	void setGain(float value);
	inline void setLacunarity(float value) {lacunarity=value;}
};

#endif