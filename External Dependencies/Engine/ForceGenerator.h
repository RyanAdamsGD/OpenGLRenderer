#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H
#include "ExportHeader.h"
class Particle;

class ENGINE_SHARED ForceGenerator
{
public:
	virtual void updateForce(Particle* particle, float dt) = 0;
};

#endif