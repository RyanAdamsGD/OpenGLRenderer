#ifndef PARTICLE_SYSTEM_FIRE_H
#define PARTICLE_SYSTEM_FIRE_H
#include "ParticleSystem.h"
#include "ExportHeader.h"

class ENGINE_SHARED ParticleSystemFire: public ParticleSystem
{
	virtual void createShader();
	virtual void fillBuffer();
	virtual void genVertexArrays();
	virtual void createFeedback();
	virtual void draw(const glm::mat4& projection,const glm::mat4& view,float dt);
public:
	ParticleSystemFire();
};

#endif