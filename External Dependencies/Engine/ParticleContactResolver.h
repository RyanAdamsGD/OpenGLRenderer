#ifndef PARTICLECONTACTRESOLVER_H
#define PARTICLECONTACTRESOLVER_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"
#include <vector>
class ParticleContact;

class ENGINE_SHARED ParticleContactResolver
{
protected:
	uint iterations;
	uint iterationsUsed;

public:
	ParticleContactResolver(uint iterations);
	inline void setIterations(uint iterations) {this->iterations = iterations;}
	void resolveContacts(ParticleContact* contactArray, uint numOfContacts, float dt);
	void resolveContacts(std::vector<ParticleContact> contactArray, float dt);
};

#endif