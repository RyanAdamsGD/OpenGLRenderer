#ifndef PARTICLE_SYSTEM_SMOKE_H
#define PARTICLE_SYSTEM_SMOKE_H
#include "ParticleSystem.h"
#include "ExportHeader.h"

class ENGINE_SHARED ParticleSystemSmoke: public ParticleSystem
{
	virtual void createShader();
	virtual void fillBuffer();
	virtual void genVertexArrays();
	virtual void createFeedback();
	virtual void draw(const glm::mat4& projection,const glm::mat4& view,float dt);
public:
	ParticleSystemSmoke();
};

#endif