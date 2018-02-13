#ifndef WOOD_H
#define WOOD_H
#include "ExportHeader.h"
#include <glm\gtx\transform.hpp>
class LatticeNoise;

enum ringAxis{ X, Y, Z};

class ENGINE_SHARED WoodTexture
{
	ringAxis axis;
	LatticeNoise* noise;
	glm::vec3 colorMin;						
	glm::vec3 colorMax;							
	float	ringFrequency;						
	float	ringUneveness;						
	float	ringNoise;							
	float 	ringNoiseFrequency;				
	float 	trunkWobble;							
	float 	trunkWobbleFrequency;
	float 	angularWobble;
	float 	angularWobbleFrequency;
	float 	grainFrequency;
	float 	grainy;								
	float 	ringy;
	glm::vec3 getColorRingX(const glm::vec3& coordinate)const;
	glm::vec3 getColorRingY(const glm::vec3& coordinate)const;
	glm::vec3 getColorRingZ(const glm::vec3& coordinate)const;

public:
	WoodTexture(void);
	virtual ~WoodTexture(void);

	virtual glm::vec3 getColor(const glm::vec3& coordinate)const;

	inline void setColorMin(const glm::vec3& value) {colorMin=value;}
	inline void setColorMax(const glm::vec3& value) {colorMax=value;}
	inline void setRingAxis(ringAxis value) {axis=value;}
	inline void setRingFrequency(float value) {ringFrequency=value;}
	inline void setRingUneveness(float value) {ringUneveness=value;}
	inline void setRingNoise(float value) {ringNoise=value;}
	inline void setRingNoiseFrequency(float value) {ringNoiseFrequency=value;}
	inline void setTrunkWobble(float value) {trunkWobble=value;}
	inline void setTrunkWobbleFrequency(float value) {trunkWobbleFrequency=value;}
	inline void setGrainFrequency(float value) {grainFrequency=value;}
	inline void setGrainy(float value) {grainy=value;}
	inline void setRingy(float value) {ringy=value;}
};

#endif