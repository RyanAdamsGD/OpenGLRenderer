#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H
#include "ExportHeader.h"
#include "glm\gtx\transform.hpp"
class Particle;


class ENGINE_SHARED ParticleContact
{

public:
	Particle* particle[2];
	float restitutionCoef;
	glm::vec3 contactNormal;
	float penetration;

	float calculateSeperatingVelocity() const;
	void resolve(float dt);
	ParticleContact(void);

private:
	void resolveVelocity(float dt);
	void resolveInterpenetration(float dt);
};

#endif