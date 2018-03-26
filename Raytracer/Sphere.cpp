#include "Sphere.h"
#include <math.h>

Vec3f Sphere::GetNormalAt(Vec3f pos)
{
	return (pos - transform.pos).Normalize();
}

bool Sphere::FindIntersection(Ray& ray, float& dist) 
{
	Vec3f delta = ray.origin - transform.pos;
	float b = ray.direction.Dot(delta);
	float c = delta.Dot(delta) - (r * r);
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