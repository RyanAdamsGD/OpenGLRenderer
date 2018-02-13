#ifndef PARTICLE_H
#define PARtICLE_H
#include "glm\gtx\transform.hpp"
#include "ExportHeader.h"

class ENGINE_SHARED Particle
{
public:
	glm::vec3 position;
	float inverseMass;
	glm::vec3 acceleration;
	glm::vec3 velocity;

	inline const glm::vec3& getPosition() const {return position;}
	inline void setPosition(const glm::vec3& position) { this->position = position;}
	inline const float& getInverseMass() const {return inverseMass;}
	inline void setInverseMass(float inverseMass) { this->inverseMass = inverseMass;}
	inline const glm::vec3& getAcceleration() const {return acceleration;}
	inline void setAcceleration(const glm::vec3& acceleration) { this->acceleration = acceleration;}
	inline const glm::vec3& getVelocity() const { return velocity;}
	inline void setVelocity(const glm::vec3& velocity) {this->velocity = velocity;}

	virtual void update(float dt) = 0;
	virtual void addForce(glm::vec3 force) {}
	Particle(const glm::vec3& position, float inverseMass, const glm::vec3& acceleration, const glm::vec3& velocity)
		:position(position),inverseMass(inverseMass),acceleration(acceleration),velocity(velocity){}

};

#endif