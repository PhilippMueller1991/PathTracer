#include "Ray.h"

#include <math.h>
#include <random>

Vector3 Ray::chooseRandomDirection(Ray::RayType rayType, Vector3 n)
{
	// Probability of choosing directions should be proportional to the cosine between
	// d and r for glossy reflections
	// d and n for diffuse reflections
	
	// TBD: Use other function for better distribution
	Vector3 dir;

	if (rayType == RayType::RAY_DIFFUSE)
	{
		float invDot = 1 - direction.Dot(n);
		
		return dir;
	}
	else if (rayType == RayType::RAY_SPECULAR)
	{
		Vector3 r = Vector3::Reflect(direction, n);
		float invDot = 1 - direction.Dot(r);

		return dir;
	}

	return dir;
}

// TODO: Change it so it returns correct oriented Vector or add additional methods to do so
Vector3 Ray::UniformSampleHemisphere(float u1, float u2)
{
    const float r = std::sqrtf(1.0f - u1 * u1);
    const float phi = 2 * std::_Pi * u2;
 
    return Vector3(std::cosf(phi) * r, std::sinf(phi) * r, u1);
}

Vector3 Ray::CosineSampleHemisphere(float u1, float u2)
{
	const float r = std::sqrtf(u1);
	const float phi = 2 * std::_Pi * u2;

	return Vector3(std::cosf(phi) * r, std::sinf(phi) * r, std::sqrtf(std::fmax(0.0f, 1.0f - u1)));
}