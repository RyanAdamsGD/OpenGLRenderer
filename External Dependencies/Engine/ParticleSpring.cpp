#include "ParticleSpring.h"
#include "Particle.h"
#include "glm\gtx\transform.hpp"

ParticleSpring::ParticleSpring(Particle* particle, float springConstant, float RestLength)
	:other(particle), k(springConstant), restLength(RestLength)
{}

void ParticleSpring::updateForce(Particle* particle, float dt)
{
	glm::vec3 force;
	force = particle->position;
	force -= other->position;

	float magnitude = glm::length(force);
	magnitude = (magnitude - restLength) * k;

	force = glm::normalize(force) * -magnitude;
	particle->addForce(force);
}
