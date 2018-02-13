#include "ParticleAnchoredSpring.h"
#include "Particle.h"

ParticleAnchoredSpring::ParticleAnchoredSpring(glm::vec3* Anchor, float springConstant, float RestLength)
	:anchor(Anchor), k(springConstant), restLength(RestLength)
{}

void ParticleAnchoredSpring::updateForce(Particle* particle, float dt)
{
	glm::vec3 force;
	force = particle->position;
	force -= *anchor;

	float magnitude = glm::length(force);
	magnitude = (magnitude - restLength) * k;

	force = glm::normalize(force) * -magnitude;
	particle->addForce(force);
}
