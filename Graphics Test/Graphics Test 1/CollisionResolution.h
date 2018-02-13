#ifndef COLLISIONRESOLUTIONn_H
#define COLLISIONRESOLUTIONn_H
#include "IUpdatable.h"
#include "PhysicsParticle.h"
#include "IParticleDrawer.h"


class CollisionResolution: public IUpdatable
{
	PhysicsParticle particle;
	float restitutionCoef;
	glm::vec3 contactColor;

	void addSliders();
	void addDrawShapes(ParticleDrawer* drawer);

public:
	CollisionResolution();
	virtual void init(ParticleDrawer* drawer);
	virtual void update(float dt);

	void collide(float dt);
	void reset();
};

#endif