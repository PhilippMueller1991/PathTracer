#pragma once

#include "Vector3.h"

class Ray
{
public:
	const enum RayType { RAY_DIFFUSE, RAY_SPECULAR, RAY_TRANSMISSION, RAY_SHADOW };
	//RayType type;

	Vector3 origin, direction;
	int bounce;
	float lastIDR;

	Ray(Vector3 origin, Vector3 direction, int bounce = 1, float refractionIndex = 1.0f)
		: origin(origin), direction(direction), bounce(bounce), lastIDR(1.0f)
	{
	}
	~Ray() {}

	Vector3 chooseRandomDirection(Ray::RayType rayType, Vector3 n);

protected:
	Vector3 UniformSampleHemisphere(float u1, float u2);
	Vector3 CosineSampleHemisphere(float u1, float u2);
};

