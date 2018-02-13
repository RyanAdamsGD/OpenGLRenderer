#include "ParticleLink.h"
#include "glm\gtx\transform.hpp"
#include "VerletParticle.h"

ParticleLink::ParticleLink(VerletParticle* mainParticle, VerletParticle* secondaryParticle, float restinglength, float stiffness, float tearLength)
	:intact(true),restingLength(restinglength),tearLength(tearLength),stiffness(stiffness)
{
	particle[0] = mainParticle;
	particle[1] = secondaryParticle;
}

void ParticleLink::solve()
{
	glm::vec3 offset = particle[0]->getPosition() - particle[1]->getPosition();
	float length = glm::length(offset);
	float offsetFromRestingLength = (restingLength - length) / length;

	if(length <= restingLength)
		return;

	stiffness = 1.0f - restingLength/length;
	//tear the cloth
	if(length > tearLength)
		intact = false;

	float stiffnessPerInvM = (stiffness / (particle[0]->getInverseMass() + particle[1]->getInverseMass()));
	offset = offset * offsetFromRestingLength * 0.5f;
	
	particle[0]->setPosition(particle[0]->getPosition() + (offset * (stiffnessPerInvM * particle[0]->getInverseMass())));
	particle[1]->setPosition(particle[1]->getPosition() - (offset * (stiffnessPerInvM * particle[1]->getInverseMass())));

}

ParticleLink::ParticleLink()
	:intact(true),restingLength(0.0f),tearLength(FLOAT_MAX),stiffness(1.0f)
{
	particle[0] = NULL;
	particle[1] = NULL;
}
	