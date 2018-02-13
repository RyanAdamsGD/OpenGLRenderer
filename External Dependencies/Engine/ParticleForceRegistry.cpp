#include "ParticleForceRegistry.h"
#include "PhysicsParticle.h"


void ParticleForceRegistry::add(PhysicsParticle* particle, ForceGenerator* fg)
{
	registration.push_back(ParticleForceRegistration(particle,fg));
}

void ParticleForceRegistry::remove(PhysicsParticle* particle, ForceGenerator* fg)
{
	//auto is like var for c++
	auto i = registration.begin();
	for(;i != registration.end(); i++)
	{
		if(&(i->fg) == &fg && &(i->particle) == &particle)
		{
			registration.erase(i);
		}
	}
}

void ParticleForceRegistry::clear()
{
	registration.clear();
}


void ParticleForceRegistry::updateForces(float dt)
{
	Registry::iterator i = registration.begin();
	for(;i != registration.end(); i++)
	{
		i->fg->updateForce(i->particle, dt);
	}
}