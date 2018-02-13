#ifndef VERLETPARTICLE_H
#define VERLETPARTICLE_H
#include "ExportHeader.h"
#include <glm\gtx\transform.hpp>
#include "Particle.h"

class ENGINE_SHARED VerletParticle: public Particle
{

public:
	glm::vec3 previousPosition;
	float damping;
	VerletParticle();
	VerletParticle(const glm::vec3& position, float inverseMass,const glm::vec3& acceleration = glm::vec3(0.0f));
	virtual void update(float dt);

};

#endif