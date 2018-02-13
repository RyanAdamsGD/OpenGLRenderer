#include "LinearNoise.h"
#include "Helper Functions.h"

LinearNoise::LinearNoise(void)
{
}

LinearNoise::LinearNoise(float gain, float lacunarity, const int numOctaves)
	:LatticeNoise(gain,lacunarity,numOctaves)
{
}

float LinearNoise::valueNoise(const glm::vec3& point)const
{
	int ix, iy, iz;
	float dx, dy, dz;
	float d[2][2][2];
	float x0, x1, x2, x3, y0, y1, z0;

	ix = FLOOR(point.x);
	dx = point.x - ix;

	iy = FLOOR(point.y);
	dy = point.y - iy;

	iz = FLOOR(point.z);
	dz = point.z - iz;

	for(int k=0;k<=1;k++)
		for(int j=0;j<=1;j++)
			for(int i=0;i<=1;i++)
				d[k][j][i] = valueTable[INDEX(ix + i,iy + j,iz + k)];

	x0 = lerp(dx, d[0][0][0], d[0][0][1]);
	x1 = lerp(dx, d[0][1][0], d[0][1][1]);
	x2 = lerp(dx, d[1][0][0], d[1][0][1]);
	x3 = lerp(dx, d[1][1][0], d[1][1][1]);
	y0 = lerp(dy, x0, x1);
	y1 = lerp(dy, x2, x3);
	z0 = lerp(dz, y0, y1);
	return z0;
}

glm::vec3 LinearNoise::vectorNoise(const glm::vec3& point)const
{
	int ix, iy, iz;
	float dx, dy, dz;
	glm::vec3 d[2][2][2];
	glm::vec3 x0, x1, x2, x3, y0, y1, z0;

	ix = FLOOR(point.x);
	dx = point.x - ix;

	iy = FLOOR(point.y);
	dy = point.y - iy;

	iz = FLOOR(point.z);
	dz = point.z - iz;

	for(int k=0;k<=1;k++)
		for(int j=0;j<=1;j++)
			for(int i=0;i<=1;i++)
				d[k][j][i] = vectorTable[INDEX(ix + i,iy + j,iz + k)];

	x0 = lerp(dx, d[0][0][0], d[0][0][1]);
	x1 = lerp(dx, d[0][1][0], d[0][1][1]);
	x2 = lerp(dx, d[1][0][0], d[1][0][1]);
	x3 = lerp(dx, d[1][1][0], d[1][1][1]);
	y0 = lerp(dy, x0, x1);
	y1 = lerp(dy, x2, x3);
	z0 = lerp(dz, y0, y1);
	return z0;
}
