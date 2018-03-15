#include "Ray.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>
#include <random>

Vec3f Ray::chooseRandomDirection(Ray::RayType rayType, Vec3f n)
{
	// Probability of choosing directions should be proportional to the cosine between
	// d and r for glossy reflections
	// d and n for diffuse reflections
	
	// TBD: Use other function for better distribution
	Vec3f dir;

	if (rayType == RayType::RAY_DIFFUSE)
	{
		float invDot = 1 - direction.dot(n);
		
		return dir;
	}
	else if (rayType == RayType::RAY_SPECULAR)
	{
		Vec3f r = Vec3f::reflect(direction, n);
		float invDot = 1 - direction.dot(r);

		return dir;
	}

	return dir;
}

// TODO: Change it so it returns correct oriented Vec3f or add additional methods to do so
Vec3f Ray::uniformSampleHemisphere(float u1, float u2)
{
    const float r = sqrtf(1.0f - u1 * u1);
    const float phi = 2 * static_cast<float>(M_PI) * u2;
 
    return Vec3f(cosf(phi) * r, sinf(phi) * r, u1);
}

Vec3f Ray::cosineSampleHemisphere(float u1, float u2)
{
	const float r = sqrtf(u1);
	const float phi = 2 * static_cast<float>(M_PI) * u2;

	return Vec3f(cosf(phi) * r, sinf(phi) * r, sqrtf(std::fmax(0.0f, 1.0f - u1)));
}