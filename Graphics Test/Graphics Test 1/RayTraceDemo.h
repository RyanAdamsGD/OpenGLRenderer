#ifndef RAYTRACEDEMO_H
#define RAYTRACEDEMO_H
#include "IUpdatable.h"
#include <vector>
#include "glm\glm.hpp"
class ParticleDrawer;


class RayTraceDemo:public IUpdatable
{
	ParticleDrawer* drawer;
	float gridSize;
	std::vector<glm::vec3> gridPoints;
	float sphereRadius;
	glm::vec3 spherePostion;
	std::vector<glm::vec3> rayPoints;

	void buildGrid();
	void addGeometry();
public:
	RayTraceDemo(void);
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);

	~RayTraceDemo(void);
};

#endif