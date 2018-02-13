#ifndef PARTICLEFORCEREGISTRY_H
#define PARTICLEFORCEREGISTRY_H
#include "ExportHeader.h"
#include <vector>
#include "ForceGenerator.h"
class PhysicsParticle;


class ENGINE_SHARED ParticleForceRegistry
{

protected:
	struct ParticleForceRegistration
	{
		PhysicsParticle* particle;
		ForceGenerator* fg;
		ParticleForceRegistration(PhysicsParticle* physicsParticle,ForceGenerator* forceGenerator)
			:particle(physicsParticle), fg(forceGenerator){}
	};
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registration;


public:

	void add(PhysicsParticle* particle, ForceGenerator* fg);
	void remove(PhysicsParticle* particle, ForceGenerator* fg);
	void clear();
	void updateForces(float dt);
};

#endif
