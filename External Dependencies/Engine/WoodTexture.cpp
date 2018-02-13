#include "WoodTexture.h"
#include "CubicNoise.h"
#include "Helper Functions.h"

WoodTexture::WoodTexture(void)
	:noise(new CubicNoise(0.5f,4.0f,2.0f)),colorMax(1.0f),colorMin(0.0f),ringFrequency(5.0f),
	ringUneveness(0.95f),ringNoise(0.01f),ringNoiseFrequency(2.0f),trunkWobble(0.35f),trunkWobbleFrequency(1.8f),
	angularWobble(0.5f),angularWobbleFrequency(1.0f),grainFrequency(45.0f),grainy(0.5f),ringy(0.5f),axis(X)
{
}

WoodTexture::~WoodTexture(void)
{
	if(noise != NULL)
	{
		delete noise;
		noise = NULL;
	}
}

glm::vec3 WoodTexture::getColor(const glm::vec3& coordinate)const
{
	switch(axis)
	{
		case X:
			return getColorRingX(coordinate);
		case Y:
			return getColorRingY(coordinate);
		case Z:
			return getColorRingZ(coordinate);
	}
}

glm::vec3 WoodTexture::getColorRingX(const glm::vec3& coordinate)const
{
	glm::vec3 hitPoint = coordinate;
	
	// perturb the hit point
	
	glm::vec3 offset = noise->vectorFBM(hitPoint * ringNoiseFrequency);

	glm::vec3 ringPoint = hitPoint + ringNoise * offset;
	
	// perturb the trunk so that it's quite along the z axis
	
	glm::vec3 tempVec = trunkWobble * noise->vectorNoise(glm::vec3(0, 0, hitPoint.y * trunkWobbleFrequency));																	
	ringPoint.z += tempVec.z;
	ringPoint.x += tempVec.x;
	
	
	// distance from the y axis
	
	float r = sqrt(ringPoint.z * ringPoint.z + ringPoint.y * ringPoint.y) * ringFrequency;
	
	
	// perturb r so that the rings aren't quite round
	
	glm::vec3 tempVec2;
	tempVec2.z = angularWobbleFrequency * ringPoint.z;
	tempVec2.x = angularWobbleFrequency * ringPoint.x * 0.1f;
	tempVec2.y = angularWobbleFrequency * ringPoint.y;
	
	float deltaR = angularWobble * smoothStep(0.0f, 5.0f, r) * noise->valueNoise(tempVec2);
	r += deltaR;
	
	
	// add some noise so that the rings are not equally spaced and have different thicknesses
	
	r += ringUneveness * noise->valueNoise(glm::vec3(r));  
	
	float temp = r;
	float inRing = smoothPulseTrain(0.1f, 0.55f, 0.7f, 0.95f, 1.0f, r);   
																		
	
	// the grain
		
	glm::vec3 grainPoint;
	grainPoint.x = hitPoint.x * grainFrequency; 
	grainPoint.y = hitPoint.y * grainFrequency * 0.05f; 
	grainPoint.z = hitPoint.z * grainFrequency; 
	
	float dpgrain 		= 0.2f; 
	float grain 		= 0.0f;
	float amplitude 	= 1.0f;    
	
	for (int i = 0; i < 2; i++) {
		float grain_valid = 1.0f - smoothStep(0.2f, 0.6f, dpgrain);
		if (grain_valid > 0.0f) {
			float g = grain_valid * noise->valueNoise(grainPoint);
			g *= (0.3f + 0.7f * inRing);
			g = pow(clamp(0.8f - g, 0.0f, 1.0f), 2.0f);
			g = grainy * smoothStep (0.5f, 1.0f, g);
			if (i == 0)
				inRing *= (1.0f - 0.4f * grain_valid);
			grain = amplitude * max(grain, g);
		}
		grainPoint = 	2.0f * grainPoint;
		dpgrain 	*= 	2.0f;
		amplitude 	*= 	0.5f;
	}
	
	float finalValue = lerp(inRing * ringy, 1.0f, grain);
	
	return lerp(finalValue,colorMax, colorMin); 
}

glm::vec3 WoodTexture::getColorRingY(const glm::vec3& coordinate)const
{
	glm::vec3 hitPoint = coordinate;
	
	// perturb the hit point
	
	glm::vec3 offset = noise->vectorFBM(hitPoint * ringNoiseFrequency);

	glm::vec3 ringPoint = hitPoint + ringNoise * offset;
	
	// perturb the trunk so that it's quite along the z axis
	
	glm::vec3 tempVec = trunkWobble * noise->vectorNoise(glm::vec3(0, 0, hitPoint.z * trunkWobbleFrequency));																	
	ringPoint.x += tempVec.x;
	ringPoint.y += tempVec.y;
	
	
	// distance from the y axis
	
	float r = sqrt(ringPoint.x * ringPoint.x + ringPoint.z * ringPoint.z) * ringFrequency;
	
	
	// perturb r so that the rings aren't quite round
	
	glm::vec3 tempVec2;
	tempVec2.z = angularWobbleFrequency * ringPoint.z;
	tempVec2.y = angularWobbleFrequency * ringPoint.y * 0.1;
	tempVec2.x = angularWobbleFrequency * ringPoint.x;
	
	float deltaR = angularWobble * smoothStep(0.0f, 5.0f, r) * noise->valueNoise(tempVec2);
	r += deltaR;
	
	
	// add some noise so that the rings are not equally spaced and have different thicknesses
	
	r += ringUneveness * noise->valueNoise(glm::vec3(r));  
	
	float temp = r;
	float inRing = smoothPulseTrain(0.1f, 0.55f, 0.7f, 0.95f, 1.0f, r);   
																		
	
	// the grain
		
	glm::vec3 grainPoint;
	grainPoint.y = hitPoint.y * grainFrequency; 
	grainPoint.x = hitPoint.x * grainFrequency * 0.05f; 
	grainPoint.z = hitPoint.z * grainFrequency; 
	
	float dpgrain 		= 0.2f; 
	float grain 		= 0.0f;
	float amplitude 	= 1.0f;    
	
	for (int i = 0; i < 2; i++) {
		float grain_valid = 1.0f - smoothStep(0.2f, 0.6f, dpgrain);
		if (grain_valid > 0.0f) {
			float g = grain_valid * noise->valueNoise(grainPoint);
			g *= (0.3f + 0.7f * inRing);
			g = pow(clamp(0.8f - g, 0.0f, 1.0f), 2.0f);
			g = grainy * smoothStep (0.5f, 1.0f, g);
			if (i == 0)
				inRing *= (1.0f - 0.4f * grain_valid);
			grain = amplitude * max(grain, g);
		}
		grainPoint = 	2.0f * grainPoint;
		dpgrain 	*= 	2.0f;
		amplitude 	*= 	0.5f;
	}
	
	float finalValue = lerp(inRing * ringy, 1.0f, grain);
	
	return lerp(finalValue,colorMax, colorMin); 
}

glm::vec3 WoodTexture::getColorRingZ(const glm::vec3& coordinate)const
{
	glm::vec3 hitPoint = coordinate;
	
	// perturb the hit point
	
	glm::vec3 offset = noise->vectorFBM(hitPoint * ringNoiseFrequency);

	glm::vec3 ringPoint = hitPoint + ringNoise * offset;
	
	// perturb the trunk so that it's quite along the z axis
	
	glm::vec3 tempVec = trunkWobble * noise->vectorNoise(glm::vec3(0, 0, hitPoint.x * trunkWobbleFrequency));																	
	ringPoint.z += tempVec.z;
	ringPoint.y += tempVec.y;
	
	
	// distance from the y axis
	
	float r = sqrt(ringPoint.x * ringPoint.x + ringPoint.y * ringPoint.y) * ringFrequency;
	
	
	// perturb r so that the rings aren't quite round
	
	glm::vec3 tempVec2;
	tempVec2.y = angularWobbleFrequency * ringPoint.y;
	tempVec2.z = angularWobbleFrequency * ringPoint.z * 0.1f;
	tempVec2.x = angularWobbleFrequency * ringPoint.x;
	
	float deltaR = angularWobble * smoothStep(0.0f, 5.0f, r) * noise->valueNoise(tempVec2);
	r += deltaR;
	
	
	// add some noise so that the rings are not equally spaced and have different thicknesses
	
	r += ringUneveness * noise->valueNoise(glm::vec3(r));  
	
	float temp = r;
	float inRing = smoothPulseTrain(0.1f, 0.55f, 0.7f, 0.95f, 1.0f, r);   
																		
	
	// the grain
		
	glm::vec3 grainPoint;
	grainPoint.y = hitPoint.y * grainFrequency; 
	grainPoint.x = hitPoint.x * grainFrequency * 0.05f; 
	grainPoint.z = hitPoint.z * grainFrequency; 
	
	float dpgrain 		= 0.2f; 
	float grain 		= 0.0f;
	float amplitude 	= 1.0f;    
	
	for (int i = 0; i < 2; i++) {
		float grain_valid = 1.0f - smoothStep(0.2f, 0.6f, dpgrain);
		if (grain_valid > 0.0f) {
			float g = grain_valid * noise->valueNoise(grainPoint);
			g *= (0.3f + 0.7f * inRing);
			g = pow(clamp(0.8f - g, 0.0f, 1.0f), 2.0f);
			g = grainy * smoothStep (0.5f, 1.0f, g);
			if (i == 0)
				inRing *= (1.0f - 0.4f * grain_valid);
			grain = amplitude * max(grain, g);
		}
		grainPoint = 	2.0f * grainPoint;
		dpgrain 	*= 	2.0f;
		amplitude 	*= 	0.5f;
	}
	
	float finalValue = lerp(inRing * ringy, 1.0f, grain);
	
	return lerp(finalValue,colorMax, colorMin); 
}
