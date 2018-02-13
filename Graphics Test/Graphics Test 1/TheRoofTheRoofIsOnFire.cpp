#include "TheRoofTheRoofIsOnFire.h"
#include "ParticleContact.h"
#include "QTDebugMenu.h"
#include "FastDelegate.h"
#include "MyTypeDefs.h"


TheRoofTheRoofIsOnFire::TheRoofTheRoofIsOnFire()
	:contactColor(glm::vec4(255,0,0,255)),restitutionCoef(1.0f),penetration(0),dt(0),velocityColor(glm::vec4(0,255,0,255))
{
	reset();
}

void TheRoofTheRoofIsOnFire::updateContactNormals()
{
	contactNormal[0] = glm::normalize(particle[1].getPosition() - particle[0].getPosition());
	contactNormal[1] = glm::normalize(particle[0].getPosition() - particle[1].getPosition());
}

void TheRoofTheRoofIsOnFire::init(ParticleDrawer* drawer)
{
	addDrawShapes(drawer);
	addSliders();
}

void TheRoofTheRoofIsOnFire::update(float dt)
{
	particle[0].update(dt);
	particle[1].update(dt);
	updateContactNormals();
	this->dt = dt;
}

void TheRoofTheRoofIsOnFire::collide()
{
	particle[2].setPosition(particle[0].getPosition());
	particle[3].setPosition(particle[1].getPosition());
	updateContactNormals();
	ParticleContact contact;
	contact.particle[0] = &particle[0];
	contact.particle[1] = &particle[1];

	contact.restitutionCoef = restitutionCoef;
	contact.contactNormal = contactNormal[0];
	contact.penetration = penetration;

	contact.resolve(dt);
}

void TheRoofTheRoofIsOnFire::reset()
{
	particle[0] = PhysicsParticle();
	particle[0].setPosition(glm::vec3(-4,-3,0));
	particle[0].setVelocity(glm::vec3(1,1,0));
	particle[1] = PhysicsParticle();
	particle[1].setPosition(glm::vec3(3,2.5,0));
	particle[1].setVelocity(glm::vec3(-1,-0.8,0));
	particle[3] = PhysicsParticle();
	particle[3].setPosition(glm::vec3(FLOAT_MAX));
	particle[3].setInverseMass(0.1f);
	particle[2] = PhysicsParticle();
	particle[2].setPosition(glm::vec3(FLOAT_MAX));
	particle[2].setInverseMass(0.1f);
}

void TheRoofTheRoofIsOnFire::addSliders()
{
	/*QtDebugMenu.addSlider("Penetration", &penetration,0,1);
	QtDebugMenu.addSlider("InverseMass Particle 1", &particle[0].inverseMass,0,0.25f);
	QtDebugMenu.addSlider("InverseMass Particle 2", &particle[1].inverseMass,0,0.25f);
	QtDebugMenu.addSlider("Restitution Coeficient", &restitutionCoef,0.1f,1.0f);
	QtDebugMenu.addButton("Collide",fastdelegate::MakeDelegate(this,&TheRoofTheRoofIsOnFire::collide));
	QtDebugMenu.addButton("Reset",fastdelegate::MakeDelegate(this,&TheRoofTheRoofIsOnFire::reset));*/
}

void TheRoofTheRoofIsOnFire::addDrawShapes(ParticleDrawer* drawer)
{
	
	
	/*drawer->addParticle(&particle[0].position,&particle[0].inverseMass);
	drawer->addParticle(&particle[1].position,&particle[1].inverseMass);
	drawer->addParticle(&particle[2].position,&particle[2].inverseMass,&contactColor);
	drawer->addParticle(&particle[3].position,&particle[3].inverseMass,&contactColor);
	drawer->addLine(&particle[0].position,&contactNormal[0],&contactColor);
	drawer->addLine(&particle[1].position,&contactNormal[1], &contactColor);
	drawer->addLine(&particle[0].position,&particle[0].velocity,&velocityColor);
	drawer->addLine(&particle[1].position,&particle[1].velocity,&velocityColor);*/
}