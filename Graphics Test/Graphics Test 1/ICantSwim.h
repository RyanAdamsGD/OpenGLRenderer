#ifndef ICANTSWIM_H
#define ICANTSWIM_H
#include "IUpdatable.h"
#include "ParticleGravity.h"
#include "ParticleForceRegistry.h"
#include "ParticleBouyancy.h"
#include "PhysicsParticle.h"
class ParticleDrawer;

class ICantSwim: public IUpdatable
{
	PhysicsParticle particle;
	ParticleForceRegistry registry;
	ParticleBouyancy boyancyForce;
	glm::vec3 waterLineLeft, waterLineRight;

public:
	ICantSwim();
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);
	void addSliders();
	void addDrawShapes(ParticleDrawer* drawer);
	void fillRegistry();
};

#endif