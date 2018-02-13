#ifndef RESTINGPARTICLES_H
#define RESTINGPARTICLES_H
#include "PhysicsParticle.h"
#include "IUpdatable.h"
#include "IParticleDrawer.h"

class RestingParticles:public IUpdatable
{
	PhysicsParticle particles[4];
	float restitutionCoef;
	glm::vec4 particleColors[4];
public:
	RestingParticles(void);
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);
	void addSliders();
	void collide(float dt, PhysicsParticle* particle1, PhysicsParticle* particle2);
	void resetPositions();
};

#endif