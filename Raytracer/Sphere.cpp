#include "Sphere.h"
#include <math.h>

Vector3 Sphere::getNormalAt(Vector3 pos)
{
	return (pos - this->pos).Normalize();
}

float Sphere::findIntersection(Ray ray) 
{
	// TBD: Renormalization necessary?
	ray.direction = ray.direction.Normalize();

	// Check if alternative calculation can be corrected for fewer calculations
	Vector3 delta = ray.origin - pos;
	float b = 2.0f * (ray.direction.Dot(delta));
	float c = delta.Dot(delta) - (r * r);

	float root = b * b - 4.0f * c;
	if (root < EPS)
		return -1;
	if (root == 0)
		return -b;

	float x0 = (-b + sqrtf(root)) / 2.0f;
	float x1 = (-b - sqrtf(root)) / 2.0f;

	return x0 < x1 ? x0 : x1;

	// Alternative calculation somehow produces other results
	//Vector3 delta = ray.origin - pos;
	//float b = ray.direction.Dot(delta);
	//float c = (delta).Dot(delta) - (r * r);
	//float root = b * b - c;
	//
	//if (root < 0)
	//	return -1;
	//if (root == 0)
	//	return -b;

	//float x0 = -b + sqrtf(root);
	//float x1 = -b - sqrtf(root);

	//return x0 < x1 ? x0 : x1;
}