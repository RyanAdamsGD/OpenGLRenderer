#include "Helper Functions.h"
#include <stdlib.h>
#include <glm\gtx\euler_angles.hpp>
#include "MyTypeDefs.h"

float randFloat(float min,float max)
{
	return float((max - min) * (float)std::rand())/(float)(RAND_MAX + min);
}

glm::mat4 align(const glm::vec3& originalOrientation, const glm::vec3& newOrientation)
{
	glm::vec3 original = glm::normalize(originalOrientation);
	float yaw = acos(glm::dot(original, glm::normalize(glm::vec3(newOrientation.x, 0, newOrientation.z)))) * _180_PI;
	float pitch = 0.0f;
	if(yaw * 0 != 0)
	{
		yaw = 0;
		pitch = acos(glm::dot(original, glm::normalize(newOrientation))) * _180_PI;
	}
	else 
	{
		if(newOrientation.x < 0)
			yaw = -yaw;
		glm::vec3 pitchVector = glm::vec3(glm::vec4(original, 0) * glm::rotate(yaw, glm::vec3(0,1,0)));
		pitch = acos(glm::dot(glm::normalize(pitchVector), glm::normalize(newOrientation))) * _180_PI;
	}
	if(pitch * 0 != 0)
	{
		pitch = 0;
	}
	else if(newOrientation.y < 0)
	{
		pitch = -pitch;
	}

	if(original.y == 0 && original.z == 0)
		return glm::rotate(-yaw, 0.0f,1.0f,0.0f) * glm::rotate(pitch, 0.0f,0.0f,1.0f);
	else
		return glm::rotate(-yaw, 0.0f,1.0f,0.0f) * glm::rotate(pitch, 1.0f,0.0f,0.0f);

}

float mod(float a, float b) 
{
	int n = (int) (a / b);

	a -= n * b;
	if (a < 0.0f)
		a += b;

	return (a);
}

float smoothPulse(float e0,float e1,float e2,float e3,float x) 
{
	return (smoothStep (e0, e1, x) - smoothStep (e2, e3, x));
}

float smoothPulseTrain(float e0,float e1,float e2,float e3,float period,float x) 
{
	return (smoothPulse(e0, e1, e2, e3, mod(x, period)));
}

float smoothStep(float a,float b,float x) 
{
	if (x < a)
		return (0.0);

	if (x >= b)
		return (1.0);

	float y = (x - a) / ( b - a);  // normalise to [0, 1]

	return (y * y * (3.0f - 2.0f * y));
}

int solveQuadric(float c[3], float s[2])
{
	float p, q, D;

	/* normal form: x^2 + px + q = 0 */

	p = c[ 1 ] / (2 * c[ 2 ]);
	q = c[ 0 ] / c[ 2 ];

	D = p * p - q;

	if (IS_ZERO(D)) 
	{
		s[ 0 ] = - p;
		return 1;
	}
	else if (D > 0) 
	{
		float sqrt_D = sqrt(D);

		s[ 0 ] =   sqrt_D - p;
		s[ 1 ] = - sqrt_D - p;
		return 2;
	}
	else /* if (D < 0) */
		return 0;
}

int solveCubic(float c[4], float s[3])
{
	int     i, num;
	double  sub;
	double  A, B, C;
	double  sq_A, p, q;
	double  cb_p, D;

	/* normal form: x^3 + Ax^2 + Bx + C = 0 */

	A = c[ 2 ] / c[ 3 ];
	B = c[ 1 ] / c[ 3 ];
	C = c[ 0 ] / c[ 3 ];

	/*  substitute x = y - A/3 to eliminate quadric term:
	x^3 +px + q = 0 */

	sq_A = A * A;
	p = 1.0/3 * (- 1.0/3 * sq_A + B);
	q = 1.0/2 * (2.0/27 * A * sq_A - 1.0/3 * A * B + C);

	/* use Cardano's formula */

	cb_p = p * p * p;
	D = q * q + cb_p;

	if (IS_ZERO(D)) 
	{
		if (IS_ZERO(q)) 
		{ /* one triple solution */
			s[ 0 ] = 0;
			num = 1;
		}
		else 
		{ /* one single and one double solution */
			double u = cbrt(-q);
			s[ 0 ] = 2 * u;
			s[ 1 ] = - u;
			num = 2;
		}
	}
	else if (D < 0) { /* Casus irreducibilis: three real solutions */
		double phi = 1.0/3 * acos(-q / sqrt(-cb_p));
		double t = 2 * sqrt(-p);

		s[ 0 ] =   t * cos(phi);
		s[ 1 ] = - t * cos(phi + PI / 3);
		s[ 2 ] = - t * cos(phi - PI / 3);
		num = 3;
	}
	else 
	{ /* one real solution */
		double sqrt_D = sqrt(D);
		double u = cbrt(sqrt_D - q);
		double v = - cbrt(sqrt_D + q);

		s[ 0 ] = u + v;
		num = 1;
	}

	/* resubstitute */

	sub = 1.0/3 * A;

	for (i = 0; i < num; ++i)
		s[ i ] -= sub;

	return num;
}

int solveQuartic(float c[5], float s[4])
{
	float  coeffs[4];
	float  z, u, v, sub;
	float  A, B, C, D;
	float  sq_A, p, q, r;
	int     i, num;

	/* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

	A = c[ 3 ] / c[ 4 ];
	B = c[ 2 ] / c[ 4 ];
	C = c[ 1 ] / c[ 4 ];
	D = c[ 0 ] / c[ 4 ];

	/*  substitute x = y - A/4 to eliminate cubic term:
	x^4 + px^2 + qx + r = 0 */

	sq_A = A * A;
	p = - 3.0/8 * sq_A + B;
	q = 1.0/8 * sq_A * A - 1.0/2 * A * B + C;
	r = - 3.0/256*sq_A*sq_A + 1.0/16*sq_A*B - 1.0/4*A*C + D;

	if (IS_ZERO(r)) 
	{
		/* no absolute term: y(y^3 + py + q) = 0 */

		coeffs[ 0 ] = q;
		coeffs[ 1 ] = p;
		coeffs[ 2 ] = 0;
		coeffs[ 3 ] = 1;

		num = solveCubic(coeffs, s);

		s[ num++ ] = 0;
	}
	else {
		/* solve the resolvent cubic ... */

		coeffs[ 0 ] = 1.0/2 * r * p - 1.0/8 * q * q;
		coeffs[ 1 ] = - r;
		coeffs[ 2 ] = - 1.0/2 * p;
		coeffs[ 3 ] = 1;

		(void) solveCubic(coeffs, s);

		/* ... and take the one real solution ... */

		z = s[ 0 ];

		/* ... to build two quadric equations */

		u = z * z - r;
		v = 2 * z - p;

		if (IS_ZERO(u))
			u = 0;
		else if (u > 0)
			u = sqrt(u);
		else
			return 0;

		if (IS_ZERO(v))
			v = 0;
		else if (v > 0)
			v = sqrt(v);
		else
			return 0;

		coeffs[ 0 ] = z - u;
		coeffs[ 1 ] = q < 0 ? -v : v;
		coeffs[ 2 ] = 1;

		num = solveQuadric(coeffs, s);

		coeffs[ 0 ]= z + u;
		coeffs[ 1 ] = q < 0 ? v : -v;
		coeffs[ 2 ] = 1;

		num += solveQuadric(coeffs, s + num);
	}

	/* resubstitute */

	sub = 1.0/4 * A;

	for (i = 0; i < num; ++i)
		s[ i ] -= sub;

	return num;
}
