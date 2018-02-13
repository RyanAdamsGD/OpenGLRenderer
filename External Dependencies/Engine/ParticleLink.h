#ifndef PARTICLELINK_H
#define PARTICLELINK_H
#include "MyTypeDefs.h"
#include "ExportHeader.h"
class VerletParticle;

class ENGINE_SHARED ParticleLink
{
public:
	VerletParticle* particle[2];
	float restingLength;
	float tearLength;
	float stiffness;

	friend class Flag;
	bool intact;
	ParticleLink();
	ParticleLink(VerletParticle* mainParticle, VerletParticle* secondaryParticle, float restinglength, float sitffness, float tearLength = FLOAT_MAX);
	void solve();
};

#endif