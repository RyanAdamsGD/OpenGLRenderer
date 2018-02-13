#ifndef PARTICLEDRAWER_H
#define PARTICLEDRAWER_H
#include "glm\gtx\transform.hpp"

class ParticleDrawer
{
protected:
	static const float DEFAULT_SCALE;
	static const bool DEFAULT_SHOW;
	static const glm::vec4 DEFAULT_COLOR;
public:
	float frameCount;
	bool useAdditiveLines;
	virtual void addParticle(const glm::vec3* position, const float* scale = &DEFAULT_SCALE, const glm::vec4* color = &DEFAULT_COLOR, const bool* showBoundBox = &DEFAULT_SHOW) = 0;
	virtual void addLine(const glm::vec3* start, const glm::vec3* end, const glm::vec4* color = &DEFAULT_COLOR) = 0;
	virtual void clearParticleList() = 0;
	virtual void clearLineList() = 0;
	virtual void removeParticle(const glm::vec3* position);
};

#endif