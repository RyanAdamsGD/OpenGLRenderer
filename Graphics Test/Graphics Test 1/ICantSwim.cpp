#include "ICantSwim.h"
#include "IParticleDrawer.h"
#include "QTDebugMenu.h"

ICantSwim::ICantSwim()
	:particle(),registry(),boyancyForce(5,10,0)
{

}

void ICantSwim::update(float dt)
{
	registry.updateForces(dt);
	//update Water Line
	waterLineLeft.y = boyancyForce.waterHeight;
	waterLineRight.y = boyancyForce.waterHeight;
}

void ICantSwim::addSliders()
{
	QtDebugMenu.addSlider("Max Depth", &boyancyForce.maxDepth, 1,10);
	QtDebugMenu.addSlider("Volume", &boyancyForce.volume,10,100);
	QtDebugMenu.addSlider("Water Height", &boyancyForce.waterHeight,-5,5);
	QtDebugMenu.addSlider("Liquid Density", &boyancyForce.liquidDesity,500,2000);
}

void ICantSwim::addDrawShapes(ParticleDrawer* drawer)
{
	/*drawer->addParticle(&particle.position);
	drawer->addParticle(&waterLineLeft);
	drawer->addParticle(&waterLineRight);*/
}

void ICantSwim::fillRegistry()
{
	registry.add(&particle,&boyancyForce);
}

void ICantSwim::init(ParticleDrawer* drawer)
{

}
