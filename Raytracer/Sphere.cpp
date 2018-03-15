#include "Sphere.h"
#include <math.h>

Vec3f Sphere::getNormalAt(Vec3f pos)
{
	return (pos - transform.pos).normalize();
}

bool Sphere::findIntersection(Ray& ray, float& dist) 
{
	Vec3f delta = ray.origin - transform.pos;
	float b = ray.direction.dot(delta);
	float c = delta.dot(delta) - (r * r);
	float root = b * b - c;
	
	if (root < EPS)
		return false;
	if (root == 0)
	{
		dist = -b;
		return true;
	}
	
	float x0 = -b + sqrtf(root);
	float x1 = -b - sqrtf(root);

	dist = x0 < x1 ? x0 : x1;
	return true;
}