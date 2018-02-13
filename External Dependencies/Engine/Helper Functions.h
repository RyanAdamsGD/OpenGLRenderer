#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#include <glm\gtx\transform.hpp>
#include "ExportHeader.h"

float ENGINE_SHARED randFloat(float min = -1.0f,float max = 1.0f);
glm::mat4 ENGINE_SHARED align(const glm::vec3& originalOrientation, const glm::vec3& newOrientation);

#define EQN_EPS     1e-30
#define IS_ZERO(x)	((x) > -EQN_EPS && (x) < EQN_EPS)
#ifndef CBRT
#define     cbrt(x)  ((x) > 0.0f ? pow((float)(x), 1.0f/3.0f) : \
			  		 ((x) < 0.0f ? -pow((float)-(x), 1.0f/3.0f) : 0.0f))
#endif

#define MIN(x,y) ((x > y) ? y : x)

#define MAX(x,y) ((x > y) ? x : y)

template<class T>
T lerp(float x,const T& a,const T& b)
{ return a + ((b-a) * x); }

inline float clamp(float x, float min, float max)
{ return x>max?max:(x<min?min:x); }

template<class T>
T fourKnotSpline(float x, const T* knots)
{
	T c3 = -0.5f * knots[0] + 1.5f * knots[1] - 1.5f * knots[2] + 0.5f * knots[3];
	T c2 = knots[0] - 2.5f * knots[1] + 2.0f * knots[2] - 0.5f * knots[3];
	T c1 = 0.5f * (-knots[0] + knots[2]);
	T c0 = knots[1];

	return ((c3 * x + c2) * x + c1) * x + c0;
}

//when using std::vector
#ifdef _VECTOR_

template<class T>
T* toArray(const std::vector<T>& source)
{
	T* ret = new T[source.size()];
	for(size_t i = 0; i < source.size(); i++)
		ret[i] = source[i];
	return ret;
}

#endif

float ENGINE_SHARED mod(float a, float b);

float ENGINE_SHARED smoothPulseTrain(float e0, float e1, float e2, float e3, float period, float x);

float ENGINE_SHARED smoothPulse(float e0,float e1,float e2,float e3,float x); 

float ENGINE_SHARED smoothStep(float a,float b,float x);

int ENGINE_SHARED solveQuadric(float c[3], float s[2]);

int ENGINE_SHARED solveCubic(float c[4], float s[3]);

int ENGINE_SHARED solveQuartic(float c[5], float s[4]);

#endif