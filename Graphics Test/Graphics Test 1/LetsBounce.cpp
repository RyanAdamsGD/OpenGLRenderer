#include "LetsBounce.h"
#include "PhysicsParticle.h"
#include "QTDebugMenu.h"
#include "IParticleDrawer.h"

LetsBounce::LetsBounce()
	:bungeeParticle(),registry(),springAnchor(0),bungeeAnchor(),springParticle(), springParticle2(),
	anchoredSpring(&springAnchor,1,1.0f), bungeeSpring(&bungeeAnchor,1,1.0f),
	spring(&springParticle,1,1.0f),spring2(&springParticle2,1,1.0f)
{
	
}

void LetsBounce::addDrawShapes(ParticleDrawer* drawer)
{
	/*drawer->addParticle(&springAnchor);
	drawer->addParticle(&bungeeParticle.position);
	drawer->addParticle(&bungeeAnchor.position);
	drawer->addParticle(&springParticle.position);
	drawer->addParticle(&springParticle2.position);*/
}

void LetsBounce::fillRegistry()
{
	registry.add(&bungeeParticle,&bungeeSpring);
	registry.add(&springParticle,&anchoredSpring);
	registry.add(&springParticle2,&spring);
	//registry.add(&springParticle,&spring2);
}

void LetsBounce::update(float dt)
{
	registry.updateForces(dt);
	bungeeParticle.update(dt);
	springParticle.update(dt);
	springParticle2.update(dt);

	//have to equalize the 2 springs
	spring2.k = spring.k;
	spring2.restLength = spring.restLength;
}

void LetsBounce::addSliders()
{
	QtDebugMenu.addSlider("Bungee Rest Length",&bungeeSpring.restLength,1.0f,10.0f);
	QtDebugMenu.addSlider("Bungee Constant",&bungeeSpring.k,1.0f,10.0f);
	QtDebugMenu.addSlider("Spring1 Rest Length", &anchoredSpring.restLength,1,10);
	QtDebugMenu.addSlider("Spring1 Constant", &anchoredSpring.k,1,10);
	QtDebugMenu.addSlider("Spring2 Rest Length", &spring.restLength,1,10);
	QtDebugMenu.addSlider("Spring2 Constant", &spring.k,1,10);
}

void LetsBounce::init(ParticleDrawer* drawer)
{
	addSliders();
	bungeeParticle.setPosition(glm::vec3(3.0f,3,0));
	bungeeAnchor.setPosition(glm::vec3(0,3,0));
	springParticle.setPosition(glm::vec3(1.1f,0,0));
	springParticle2.setPosition(glm::vec3(3.1f,0,0));	
	addDrawShapes(drawer);
	fillRegistry();
}
