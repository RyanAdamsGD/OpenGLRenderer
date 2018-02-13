#ifndef HAIR_H
#define HAIR_H
#include "IUpdatable.h"
#include "ParticleContactResolver.h"
#include <vector>
#include "MyTypeDefs.h"
#include "glm\glm.hpp"
#include "Grid.h"
class ParticleDrawer;
class VerletParticle;
class ParticleLink;

class Hair:public IUpdatable
{
	ParticleDrawer* drawer;

	glm::vec3 centerOfHead;
	
	float headXScale, headRadius, browAngle;
	float restingLength, stiffness, inverseMass, keyHairPercent;
	uint keyHairCount, hairLength, normHairCount, numColumns, numRows;

	Grid<VerletParticle> gridKeyHairs;
	Grid<glm::vec3> gridVelocities;

	struct HairStrand
	{
		VerletParticle* root;
		VerletParticle* hairParticles;
		ParticleLink* links;
		int count;

		void update(float dt);
		void moveAwayFromPoint(glm::vec3 point, float minDistanceSq, float dt, float headXScale);
		~HairStrand();
		HairStrand(VerletParticle* root, VerletParticle* hairParticles, ParticleLink* links, int count);
	};
	std::vector<HairStrand*> hairs;
	std::vector<HairStrand*> keyHairs;

	glm::vec3 getPosition(uint index, uint numColumn, uint numRow);
	void writeFile();
	char* getFileName() const;
	void updateVelocityGrid();
	void updateKeyHairs();
	void updateHairsVelocity();
	void createHair(std::vector<HairStrand*>& hair, uint numColumn, uint numRow);
public:
	Hair(void);
	virtual void update(float dt);
	virtual void init(ParticleDrawer* drawer);
	void addSliders();
	void addParticles();
};

#endif