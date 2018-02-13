#include "RestingParticles.h"
#include "MyTypeDefs.h"
#include "glm\gtx\transform.hpp"
#include "ParticleContact.h"

RestingParticles::RestingParticles(void)
	:restitutionCoef(0.3f)
{
	resetPositions();
	particleColors[0] = glm::vec4(255,0,0,255);
	particleColors[2] = glm::vec4(0,255,0,255);
	particleColors[3] = glm::vec4(0,0,255,255);
	particleColors[4] = glm::vec4(255,255,0,255);
}


void RestingParticles::update(float dt)
{
	

	for(uint i=0;i<4;i++)
	{
		PhysicsParticle& baseP = particles[i];
		for(uint j=0;j<4;j++)
		{
			PhysicsParticle& secondP = particles[j];
			if(i!=j)
			{
				float len = glm::length(baseP.getPosition() - secondP.getPosition());
				float dis = (baseP.getMass() + secondP.getMass())*0.025f;
				if(len<dis)
				{
					collide(dt,&particles[i],&particles[j]);
				}
			}
		}
		if(baseP.getPosition().y <=0.26f)
		{
			collide(dt,&particles[i],NULL);
		}
	}

	for(uint i=0;i<4;i++)
	{
		particles[i].update(dt);
	}
}

void RestingParticles::init(ParticleDrawer* drawer)
{
	/*drawer->addParticle(&particles[0].position,&particles[0].inverseMass,&particleColors[0]);
	drawer->addParticle(&particles[1].position,&particles[1].inverseMass,&particleColors[1]);
	drawer->addParticle(&particles[2].position,&particles[2].inverseMass,&particleColors[2]);
	drawer->addParticle(&particles[3].position,&particles[3].inverseMass,&particleColors[3]);*/
}

void RestingParticles::addSliders()
{

}

void RestingParticles::collide(float dt, PhysicsParticle* particle1, PhysicsParticle* particle2)
{
	ParticleContact contact;
	contact.particle[0] = particle1;
	contact.particle[1] = particle2;

	contact.restitutionCoef = restitutionCoef;
	glm::vec3 vectorBetween;
	if(particle2 != NULL)
		vectorBetween = particle1->getPosition() - particle2->getPosition();
	else
		vectorBetween = glm::vec3(0.0f,particle1->getPosition().y,0.0f);

		contact.contactNormal = glm::normalize(vectorBetween);
	if(particle2 != NULL)
		contact.penetration = -glm::length(vectorBetween) + (particle1->getMass() + particle2->getMass()) * 0.025f;
	else
		contact.penetration = -glm::length(vectorBetween) + particle1->getMass() * 0.025f;

	contact.resolve(dt);
}

void RestingParticles::resetPositions()
{
	particles[0] = PhysicsParticle(glm::vec3(-9.0f,4.5f,0.0f),glm::vec3(3.0f,1.0f,0.0f),glm::vec3(0.0f,-0.9f,0.0f));
	particles[1] = PhysicsParticle(glm::vec3(2.0f,6.5f,0.0f),glm::vec3(0.0f),glm::vec3(0.0f,-0.9f,0.0f));
	particles[2] = PhysicsParticle(glm::vec3(2.0f,4.5f,0.0f),glm::vec3(0.0f),glm::vec3(0.0f,-0.9f,0.0f));
	particles[3] = PhysicsParticle(glm::vec3(2.0f,1.5f,0.0f),glm::vec3(0.0f),glm::vec3(0.0f,-0.9f,0.0f));
}
