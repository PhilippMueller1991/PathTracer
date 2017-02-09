#pragma once

#include "Vector3.h"

class Ray
{
public:
	const enum RayType { RAY_DIFFUSE, RAY_SPECULAR, RAY_TRANSMISSION, RAY_SHADOW };

	RayType type;
	Vector3 origin, direction;
	int bounce;

	Ray() : type(RAY_DIFFUSE), origin(Vector3(0, 0, 0)), direction(Vector3(1, 0, 0)) {}
	Ray(Vector3 origin, Vector3 direction) : type(RAY_DIFFUSE), origin(origin), direction(direction) {}
	Ray(Vector3 origin, Vector3 direction, RayType type, int bounce) : origin(origin), direction(direction), type(type), bounce(bounce) {}
	~Ray() {}


	Vector3 chooseRandomDirection(Ray::RayType rayType, Vector3 n);

protected:
	Vector3 UniformSampleHemisphere(float u1, float u2);
	Vector3 CosineSampleHemisphere(float u1, float u2);
};

