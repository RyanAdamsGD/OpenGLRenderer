#ifndef PARTICLEDRAG_H
#define PARTICLEDRAG_H
#include "ExportHeader.h"
#include "ForceGenerator.h"

class ENGINE_SHARED ParticleDrag: public ForceGenerator
{
	
public:
	float k1;
	float k2;
	ParticleDrag(float slowDragCoef, float fastDragCoef);
	virtual void updateForce(Particle* particle, float dt);
};

#endif