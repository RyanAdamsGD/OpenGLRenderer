#ifndef PARTICLEBOYANCY_H
#define PARTICLEBOYANCY_H
#include "ForceGenerator.h"
#include "ExportHeader.h"
class Particle;


class ENGINE_SHARED ParticleBouyancy: public ForceGenerator
{
public:
	float maxDepth;
	float volume;
	float waterHeight;
	float liquidDesity;

	ParticleBouyancy(float MaxDeapth,float objectsVolume,float WaterHeight, float Density = 1000.0f);
	virtual void updateForce(Particle* particle, float dt);
};

#endif