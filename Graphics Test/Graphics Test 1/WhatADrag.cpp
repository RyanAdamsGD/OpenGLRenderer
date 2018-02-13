#include "WhatADrag.h"
#include "PhysicsParticle.h"
#include "QtPaintWindow.h"
#include "QTDebugMenu.h"
#include "IParticleDrawer.h"

WhatADrag::WhatADrag()
	:force(1,0),particle(),registry(),velocity(10)
{
	
}

void WhatADrag::update(float dt)
{
	registry.updateForces(dt);
	if(particle.getPosition().x > 10 || particle.getVelocity().x < 0.0f)
	{
		/*particle.position.x = 0;
		particle.velocity.x = velocity;*/
	}
	particle.update(dt);
}


void WhatADrag::addSliders()
{
	QtDebugMenu.addSlider("K1",&force.k1,0.0f,30.0f);
	QtDebugMenu.addSlider("K2",&force.k2,0.0f,30.0f);
	QtDebugMenu.addSlider("Velocity", &velocity,1,10);
}

void WhatADrag::init(ParticleDrawer* drawer)
{
	/*drawer->addParticle(&particle.position);
	addSliders();
	particle.position.x = 10;
	particle.setVelocity(glm::vec3(velocity,0,0));
	registry.add(&particle,&force);*/
}