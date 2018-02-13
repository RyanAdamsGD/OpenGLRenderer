#include "ParticleBungee.h"
#include "Particle.h"


ParticleBungee::ParticleBungee(Particle* Particle, float springConstant, float RestLength)
	:other(Particle), k(springConstant), restLength(RestLength)
{}

void ParticleBungee::updateForce(Particle* particle, float dt)
{
	glm::vec3 force;
	force = particle->position;
	force -= other->getPosition();

	float magnitude = glm::length(force);

	if(magnitude <= restLength)
		return;

	magnitude = k * (restLength - magnitude);

	force = glm::normalize(force) * magnitude;
	particle->addForce(force);
}

