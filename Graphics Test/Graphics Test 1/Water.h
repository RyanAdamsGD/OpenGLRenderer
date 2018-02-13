#ifndef WATER_H
#define WATER_H
#include "IUpdatable.h"
#include "LinkRegistery.h"
#include <vector>
#include "MyTypeDefs.h"
#include "glm\glm.hpp"
class ParticleDrawer;
class VerletParticle;
class ParticleLink;

class Water:public IUpdatable
{
	VerletParticle** particles;
	ParticleDrawer* drawer;

	float restingLength;
	float newWidth, newHeight;
	uint width;
	uint height;

	struct waterEffect
	{
		static const float travelSpeed;
		static const float maxDuration;
		float scale;
		float duration;
		glm::vec3 position;
		waterEffect(const glm::vec3& position, float scale):duration(0),position(position),scale(scale){}
	};
	std::vector<waterEffect> effects;

	std::vector<VerletParticle*> rain;

	void updateWaterEffect(waterEffect& effect, float dt);
	void addRainDrop();
	void writeParticlePosition()const;
	char* getFileName()const;
public:
	Water(void);
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);
	void createWater();
	void addSliders();
	void addParticles();
};

#endif