#include "ParticleContactResolver.h"
#include "ParticleContact.h"

ParticleContactResolver::ParticleContactResolver(uint iterations)
	:iterations(iterations), iterationsUsed(0)
{
}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, uint numOfContacts, float dt)
{
	iterationsUsed = 0;
	while(iterationsUsed < iterations)
	{
		float max = 0x7FFFFF;
		uint maxIndex = numOfContacts;
		for(uint i=0; i< numOfContacts;i++)
		{
			float sepVel = contactArray[i].calculateSeperatingVelocity();
			if(sepVel < max && (sepVel < 0 || contactArray[i].penetration > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		if(maxIndex == numOfContacts) 
			break;

		contactArray[maxIndex].resolve(dt);

		iterationsUsed++;
	}
}

void ParticleContactResolver::resolveContacts(std::vector<ParticleContact> contactArray, float dt)
{
	uint numOfContacts = contactArray.size();
	iterationsUsed = 0;
	while(iterationsUsed < iterations)
	{
		float max = 0x7FFFFF;
		uint maxIndex = numOfContacts;
		for(uint i=0; i< numOfContacts;i++)
		{
			float sepVel = contactArray[i].calculateSeperatingVelocity();
			if(sepVel < max && (sepVel < 0 || contactArray[i].penetration > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		if(maxIndex == numOfContacts) 
			break;

		contactArray[maxIndex].resolve(dt);

		iterationsUsed++;
	}
}

