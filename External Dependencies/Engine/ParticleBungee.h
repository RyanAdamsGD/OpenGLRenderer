#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H
#include "ForceGenerator.h"
#include "ExportHeader.h"
class Particle;

class ENGINE_SHARED ParticleBungee: public ForceGenerator
{
	Particle* other;

public:
	float restLength;
	float k;
	ParticleBungee(Particle* Particle, float springConstant, float RestLength);
	virtual void updateForce(Particle* particle, float dt);
};

#endif