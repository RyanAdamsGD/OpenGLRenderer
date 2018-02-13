#include "ParticleBouyancy.h"
#include "PhysicsParticle.h"
#include "glm\gtx\transform.hpp"


ParticleBouyancy::ParticleBouyancy(float MaxDeapth,float objectsVolume,float WaterHeight,float Density)
	:maxDepth(MaxDeapth), volume(objectsVolume), waterHeight(WaterHeight),liquidDesity(Density)
{
}

void ParticleBouyancy::updateForce(Particle* particle, float dt)
{
	float depth = particle->position.y;

	if(depth >= waterHeight)
		return;
	glm::vec3 force(0);

	if(depth <= waterHeight - maxDepth)
	{
		force.y = liquidDesity * volume;
		particle->addForce(force);
		return;
	}

	force.y = liquidDesity * volume * (depth - maxDepth - waterHeight)/2 * maxDepth;
	particle->addForce(force);
}
