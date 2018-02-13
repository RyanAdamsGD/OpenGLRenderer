#include "CollisionResolution.h"
#include "ParticleContact.h"
#include "ParticleContactResolver.h"
#include "QTDebugMenu.h"
#include "FastDelegate.h"


CollisionResolution::CollisionResolution():contactColor(glm::vec3(255,0,0)),restitutionCoef(1.0f)
{
	reset();
}

void CollisionResolution::init(ParticleDrawer* drawer)
{
	addDrawShapes(drawer);
	addSliders();
}

void CollisionResolution::update(float dt)
{
	particle.update(dt);
	if(particle.position.y <= 0)
	{
		collide(dt);
	}
}

void CollisionResolution::collide(float dt)
{
	ParticleContact contact;
	contact.particle[0] = &particle;
	contact.particle[1] = NULL;

	contact.restitutionCoef = restitutionCoef;
	contact.contactNormal = glm::vec3(0.0f,1.0f,0.0f);
	contact.penetration = 0 - particle.position.y;

	contact.resolve(dt);
}

void CollisionResolution::reset()
{
	particle = PhysicsParticle();
	particle.position = glm::vec3(-5,3,5);
	particle.velocity = glm::vec3(1,-5,1);
	particle.acceleration = glm::vec3(0.0f,-0.9f,0.0f);
}

void CollisionResolution::addSliders()
{
	QTDebugMenu::addSlider("InverseMass Particle 1", &particle.inverseMass,0,0.25f);
	QTDebugMenu::addSlider("Restitution Coeficient", &restitutionCoef,0.1f,1.0f);
	QTDebugMenu::addButton("Reset",fastdelegate::MakeDelegate(this,&CollisionResolution::reset));
}

void CollisionResolution::addDrawShapes(ParticleDrawer* drawer)
{
	drawer->addParticle(&particle.position,&particle.inverseMass);
	//drawer->addLine(&particle.position,&particle.velocity);
}

