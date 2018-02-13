#include "LinkRegistery.h"
#include "ParticleLink.h"

LinkRegistery::LinkRegistery(void)
	:registration()
{
}

void LinkRegistery::add(const VerletParticle* mainParticle,const VerletParticle* secondaryParticle, ParticleLink* link)
{
	registration.push_back(ParticleLinkRegistration(mainParticle, link));
}

void LinkRegistery::remove(const VerletParticle* particle, ParticleLink* link)
{
	Registry::iterator i = registration.begin();
	for(;i != registration.end();i++)
	{
		if(&(i->particle) == &particle && &(i->link) == &link)
		{
			registration.erase(i);
		}
	}
}

void LinkRegistery::clear()
{
	registration.clear();
}

void LinkRegistery::solveLinks()
{
	Registry::iterator i = registration.begin();
	for(;i != registration.end();i++)
	{
		i->link->solve();
	}
}
