#ifndef PARTICLEGRAVITY_H
#define PARTICLEGRAVITY_H
#include "ExportHeader.h"
#include "ForceGenerator.h"
#include "glm\gtx\transform.hpp"

class ENGINE_SHARED ParticleGravity: public ForceGenerator
{
	glm::vec3 gravity;
public:
	ParticleGravity(const glm::vec3& g);
	virtual void updateForce(Particle* particle, float dt);
};

#endif