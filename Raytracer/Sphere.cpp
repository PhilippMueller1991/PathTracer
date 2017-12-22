#include "Sphere.h"
#include <math.h>

Vector3 Sphere::getNormalAt(Vector3 pos)
{
	return (pos - transform.pos).Normalize();
}

float Sphere::findIntersection(Ray ray) 
{
	Vector3 delta = ray.origin - transform.pos;
	float b = ray.direction.Dot(delta);
	float c = delta.Dot(delta) - (r * r);
	float root = b * b - c;
	
	if (root < EPS)
		return -1;
	if (root == 0)
		return -b;

	float x0 = -b + sqrtf(root);
	float x1 = -b - sqrtf(root);

	return x0 < x1 ? x0 : x1;
}