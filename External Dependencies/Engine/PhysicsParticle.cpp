#include "physicsParticle.h"
#include "MyAssert.h"
#include "MyTypeDefs.h"

PhysicsParticle::PhysicsParticle(const glm::vec3& position, const glm::vec3& velocity, glm::vec3 acceleration, float damping, float inverseMass)
	:Particle(position,inverseMass,acceleration,velocity),damping(damping),forceAccum(0.0f)
{
}

void PhysicsParticle::update(float dt)
{
	if(!hasFiniteMass())
		return;
	
	assert(dt > 0.0f);

	position += velocity * dt;

	glm::vec3 resultingAcc = acceleration;
	resultingAcc += forceAccum * inverseMass;

	velocity += resultingAcc * dt;
	velocity *= glm::pow(damping, dt);

	clearAccumlator();
}

void PhysicsParticle::addForce(const glm::vec3& force)
{
	forceAccum += force;
}

void PhysicsParticle::clearAccumlator()
{
	forceAccum = glm::vec3(0.0f);
}
