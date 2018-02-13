#include "ParticleDrag.h"
#include "Particle.h"
#include "glm\gtx\transform.hpp"
using namespace glm;

ParticleDrag::ParticleDrag(float slowDragCoef, float fastDragCoef)
	:k1(slowDragCoef), k2(fastDragCoef)
{}

void ParticleDrag::updateForce(Particle* particle, float dt)
{
	vec3 force = particle->velocity;

	float dragCoef = force.length();
	dragCoef = k1 * dragCoef + k2 * dragCoef * dragCoef;

	if(dragCoef != 0.0f)
	{
		force = glm::normalize(force);
		force *= -dragCoef;
		particle->addForce(force);
	}
}

