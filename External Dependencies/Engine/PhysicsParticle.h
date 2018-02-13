#ifndef PHYSICSPARTICLE_H
#define PHYSICSPARTICLE_H
#include "ExportHeader.h"
#include <glm\gtx\transform.hpp>
#include "Particle.h"

class ENGINE_SHARED PhysicsParticle:public Particle
{
	float damping;
	glm::vec3 forceAccum;
	friend class CaptureTheFlag;
	friend class LevelEditor;

public:	
	
	void addForce(const glm::vec3& force);
	void clearAccumlator();
	virtual void update(float dt);
	PhysicsParticle(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f), float damping= 0.8f, float inverseMass = 0.05f);

	inline bool hasFiniteMass() const {return inverseMass > 0.0f;}
	inline float getMass() const {return 1/inverseMass;}
};

#endif