#include "CubicNoise.h"
#include "Helper Functions.h"

CubicNoise::CubicNoise(void)
{
}

CubicNoise::CubicNoise(float gain, float lacunarity, const int numOctaves)
	:LatticeNoise(gain,lacunarity,numOctaves)
{
}

float CubicNoise::valueNoise(const glm::vec3& point)const
{
	int ix, iy, iz;
	float dx, dy, dz;
	float xKnots[4], yKnots[4], zKnots[4];

	ix = FLOOR(point.x);
	dx = point.x - ix;

	iy = FLOOR(point.y);
	dy = point.y - iy;

	iz = FLOOR(point.z);
	dz = point.z - iz;

	for(int k=-1;k<=2;k++)
	{
		for(int j=-1;j<=2;j++)
		{
			for(int i=-1;i<=2;i++)
			{
				xKnots[i+1] = valueTable[INDEX(ix + i, iy + j, iz + k)];
			}
			yKnots[j+1] = fourKnotSpline(dx,xKnots);
		}
		zKnots[k+1] = fourKnotSpline(dy,yKnots);
	}

	return clamp(fourKnotSpline(dz,zKnots),-1.0,1.0);
}

glm::vec3 CubicNoise::vectorNoise(const glm::vec3& point)const
{
	int ix, iy, iz;
	float dx, dy, dz;
	glm::vec3 xKnots[4], yKnots[4], zKnots[4];

	ix = FLOOR(point.x);
	dx = point.x - ix;

	iy = FLOOR(point.y);
	dy = point.y - iy;

	iz = FLOOR(point.z);
	dz = point.z - iz;

	for(int k=-1;k<=2;k++)
	{
		for(int j=-1;j<=2;j++)
		{
			for(int i=-1;i<=2;i++)
			{
				xKnots[i+1] = vectorTable[INDEX(ix + i, iy + j, iz + k)];
			}
			yKnots[j+1] = fourKnotSpline(dx,xKnots);
		}
		zKnots[k+1] = fourKnotSpline(dy,yKnots);
	}

	return fourKnotSpline(dz,zKnots);
}