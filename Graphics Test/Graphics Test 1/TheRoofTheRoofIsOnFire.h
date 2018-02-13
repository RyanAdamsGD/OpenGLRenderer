#ifndef THEROOFTHEROOFISONFIRE_H
#define THEROOFTHEROOFISONFIRE_H
#include "IUpdatable.h"
#include "PhysicsParticle.h"
#include "IParticleDrawer.h"

class TheRoofTheRoofIsOnFire:public IUpdatable
{
	PhysicsParticle particle[4];
	glm::vec3 contactNormal[2];
	float restitutionCoef, penetration, dt;
	glm::vec4 contactColor;
	glm::vec4 velocityColor;

	void updateContactNormals();
	void addSliders();
	void addDrawShapes(ParticleDrawer* drawer);

public:
	TheRoofTheRoofIsOnFire(void);
	virtual void init(ParticleDrawer* drawer);
	virtual void update(float dt);

	void collide();
	void reset();
};

#endif