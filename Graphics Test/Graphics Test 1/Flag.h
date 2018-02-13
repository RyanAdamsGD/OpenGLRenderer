#ifndef FLAG_H
#define FLAG_H
#include "IUpdatable.h"
#include "LinkRegistery.h"
#include <vector>
#include "MyTypeDefs.h"
class ParticleDrawer;
class VerletParticle;
class ParticleLink;

class Flag:public IUpdatable
{
	bool goLeft;
	VerletParticle** particles;
	ParticleLink* links;
	LinkRegistery registry;
	float particlesMass;
	float restingLength;
	float stiffness;
	float newWidth, newHeight;
	uint width;
	uint height;
	ParticleDrawer* drawer;
	float numOfSolves;
	bool pinTop, pinCorners;
	float fanStrength;
	float fanX;
	VerletParticle *fanParticle, *directionParticle;
public:
	Flag(void);
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);
	void createFlag();
	void addSliders();
	void addParticles();
};

#endif