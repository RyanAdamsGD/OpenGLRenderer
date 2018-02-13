#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H
#include "ForceGenerator.h"
#include "ExportHeader.h"

class ENGINE_SHARED ParticleSpring: public ForceGenerator
{
	Particle* other;

public:
	float k;
	float restLength;

	ParticleSpring(Particle* particle, float springConstant, float RestLength);
	virtual void updateForce(Particle* particle, float dt);
};

#endif