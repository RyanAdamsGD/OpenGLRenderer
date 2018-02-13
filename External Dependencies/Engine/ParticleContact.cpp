#include "ParticleContact.h"
#include "Particle.h"

ParticleContact::ParticleContact(void)
{
}

void ParticleContact::resolve(float dt)
{
	//resolveInterpenetration(dt);
	resolveVelocity(dt);
}

void ParticleContact::resolveInterpenetration(float dt)
{
	if(penetration <= 0)
		return;

	float totalInverseMass = particle[0]->getInverseMass();
	if(particle[1])
		totalInverseMass += particle[1]->getInverseMass();

	if(totalInverseMass <= 0)
		return;

	glm::vec3 movePerIMass = contactNormal * ( penetration / totalInverseMass);

	particle[0]->setPosition(particle[0]->getPosition() + movePerIMass * particle[0]->getInverseMass());

	if(particle[1])
	{
		particle[1]->setPosition(particle[1]->getPosition() + movePerIMass * -particle[1]->getInverseMass());
	}
}


float ParticleContact::calculateSeperatingVelocity() const
{
	glm::vec3 relativeVelocity = particle[0]->getVelocity();
	//check if its null
	if(particle[1])
		relativeVelocity -= particle[1]->getVelocity();
	return glm::dot(relativeVelocity, contactNormal);
}

void ParticleContact::resolveVelocity(float dt)
{
	float seperatingVelocity = calculateSeperatingVelocity();

	//angle between contact normal and  relative velocity is acute
	//objects are already seperating
	if(seperatingVelocity > 0.0f)
	{
		return;
	}

	float newSeperatingVelocity = -seperatingVelocity * restitutionCoef;

	glm::vec3 accCausedVelocity = particle[0]->getAcceleration();
	if(particle[1])
		accCausedVelocity -= particle[1]->getAcceleration();

	float accCausedSepVelocity = glm::dot(accCausedVelocity, contactNormal) * dt;

	if(accCausedSepVelocity < 0)
	{
		newSeperatingVelocity += restitutionCoef * accCausedSepVelocity;
		if(newSeperatingVelocity < 0)
			newSeperatingVelocity = 0;
	}

	float deltaVelocity = newSeperatingVelocity - seperatingVelocity;

	float totalInverseMass = particle[0]->getInverseMass();
	if(particle[1])
		totalInverseMass += particle[1]->getInverseMass();

	

	//both are infinite masses return
	if(totalInverseMass <= 0)
		return;

	float impulse = deltaVelocity/totalInverseMass;

	glm::vec3 impulsePerMass = contactNormal * impulse;

	particle[0]->setVelocity( particle[0]->getVelocity() + impulsePerMass * particle[0]->getInverseMass());

	if(particle[1])
	{
		particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerMass * -particle[1]->getInverseMass());
	}

}

