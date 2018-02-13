#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "ExportHeader.h"
#include "glm\glm.hpp"
#include "MyTypeDefs.h"


class ENGINE_SHARED ParticleSystem
{
protected:
	float POINTSIZE;
	float LIFETIME;
	float SPEED;

	uint feedback[2];
	uint initVel;
	uint posBuf[2];
	uint velBuf[2];
	uint startTime[2];
	uint drawBuf;
	uint texID;
	uint programID;
	uint particleCount;
	uint particleArray[2];
	uint textureID;

	float lifeTime;
	float runTime;
	glm::vec3 position;

	void compileShader(const uint& shaderID, const char* fileName);
	virtual void createShader() = 0;
	virtual void fillBuffer() =0;
	virtual void genVertexArrays() = 0;
	virtual void createFeedback() = 0;
	virtual void draw(const glm::mat4& projection,const glm::mat4& view,float dt) = 0;
	void loadTexture(const char* filename);

public:
	ParticleSystem(void):drawBuf(0),runTime(0){}
	void createParticleSystem(const glm::vec3& pos,uint numOfParticles);
	void drawParticles(const glm::mat4& projection,const glm::mat4& view,float dt);
};

#endif