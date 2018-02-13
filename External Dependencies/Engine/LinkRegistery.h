#ifndef LINKREGISTRY_H
#define LINKREGISTRY_H
#include "ExportHeader.h"
#include <vector>

class VerletParticle;
class ParticleLink;

class ENGINE_SHARED LinkRegistery
{
	struct ParticleLinkRegistration
	{
		const VerletParticle* particle;
		ParticleLink* link;
		ParticleLinkRegistration(const VerletParticle* particle,ParticleLink* link)
			:particle(particle), link(link){}
	};
	typedef std::vector<ParticleLinkRegistration> Registry;
	Registry registration;


public:
	LinkRegistery();
	void add(const VerletParticle* mainParticle,const VerletParticle* secondaryParticle, ParticleLink* link);
	void remove(const VerletParticle* particle, ParticleLink* link);
	void clear();
	void solveLinks();
};

#endif