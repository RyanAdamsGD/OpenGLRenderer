#include "VerletParticle.h"


VerletParticle::VerletParticle(const glm::vec3& position, float inverseMass,const glm::vec3& acceleration)
	:Particle(position,inverseMass,acceleration,glm::vec3(0.0f)), previousPosition(position),damping(0.96f)
{
}

void VerletParticle::update(float dt)
{
	glm::vec3 verVelocity = position -  previousPosition;
	velocity *= damping;
	verVelocity += velocity * dt;
	verVelocity *= damping;
	previousPosition = position;
	position += verVelocity + 0.5f * acceleration * inverseMass * dt;
}
VerletParticle::VerletParticle()
	:Particle(glm::vec3(0.0f),0.0f,glm::vec3(0.0f),glm::vec3(0.0f))
{
}
