#pragma once

#include "Vector.h"

class Ray
{
public:
	const enum RayType { RAY_DIFFUSE, RAY_SPECULAR, RAY_TRANSMISSION, RAY_SHADOW };	// Not used atm
	//RayType type;

	Vec3f origin, direction;
	int bounce;
	float lastIDR;

	Ray(Vec3f origin, Vec3f direction, int bounce = 1, float refractionIndex = 1.0f)
		: origin(origin), direction(direction), bounce(bounce), lastIDR(1.0f)
	{
	}
	~Ray() {}

	Vec3f ChooseRandomDirection(Ray::RayType rayType, Vec3f n);

protected:
	Vec3f UniformSampleHemisphere(float u1, float u2);
	Vec3f CosineSampleHemisphere(float u1, float u2);
};

