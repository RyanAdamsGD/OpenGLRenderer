#ifndef LETSBOUNCE_H
#define LETSBOUNCE_H
#include "IUpdatable.h"
#include "ParticleGravity.h"
#include "ParticleForceRegistry.h"
#include "ParticleBungee.h"
#include "ParticleSpring.h"
#include "ParticleAnchoredSpring.h"
#include "PhysicsParticle.h"
class ParticleDrawer;


class LetsBounce:public IUpdatable
{
	glm::vec3 springAnchor;
	PhysicsParticle bungeeParticle;
	PhysicsParticle bungeeAnchor;
	PhysicsParticle springParticle;
	PhysicsParticle springParticle2;
	ParticleForceRegistry registry;
	ParticleAnchoredSpring anchoredSpring;
	ParticleBungee bungeeSpring;
	ParticleSpring spring;
	ParticleSpring spring2;

public:
	LetsBounce();
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);
	void addSliders();
	void addDrawShapes(ParticleDrawer* drawer);
	void fillRegistry();
};

#endif