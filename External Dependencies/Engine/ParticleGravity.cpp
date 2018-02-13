#include "ParticleGravity.h"
#include "Particle.h"

ParticleGravity::ParticleGravity(const glm::vec3& g):gravity(g)
{}

void ParticleGravity::updateForce(Particle* particle, float dt)
{
	if(!particle->inverseMass)
		return;

	particle->addForce(gravity / particle->inverseMass);
}