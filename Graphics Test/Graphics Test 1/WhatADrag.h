#ifndef WHATADRAG
#define WHATADRAG
#include "IUpdatable.h"
#include "ParticleGravity.h"
#include "ParticleForceRegistry.h"
#include "ParticleDrag.h"
#include "PhysicsParticle.h"
class ParticleDrawer;

class WhatADrag:public IUpdatable
{
	PhysicsParticle particle;
	ParticleDrag force;
	ParticleForceRegistry registry;
	float velocity;
	
public:
	WhatADrag();
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);
	void addSliders();
};

#endif