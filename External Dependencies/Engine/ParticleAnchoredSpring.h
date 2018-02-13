#ifndef PARTICLEANCHOREDSPRING_H
#define PARTICLEANCHOREDSPRING_H
#include "ForceGenerator.h"
#include "ExportHeader.h"
#include "glm\gtx\transform.hpp"

class ENGINE_SHARED ParticleAnchoredSpring: public ForceGenerator
{
	glm::vec3* anchor;

public:
	float k;
	float restLength;

	ParticleAnchoredSpring(glm::vec3* Anchor, float springConstant, float RestLength);
	virtual void updateForce(Particle* particle, float dt);
};

#endif