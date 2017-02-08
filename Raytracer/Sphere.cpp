#include "Sphere.h"
#include <math.h>

Vector3 Sphere::getNormalAt(Vector3 pos)
{
	return (pos - this->pos).Normalize();
}

float Sphere::findIntersection(Ray ray) 
{
	// Assume ray direction is normalized
	Vector3 diff = ray.origin - pos;
	float rayDiffProj = ray.direction.Dot(diff);
	float lengthDiff = (diff).Magnitude();
	float underRoot = rayDiffProj * rayDiffProj - lengthDiff * lengthDiff + r;

	if (underRoot < 0)	// Ray does not intersect with sphere
		return -1;
	if(underRoot == 0)	// Ray touches sphere exactly once
		return -rayDiffProj;
	// Ray intersects with sphere two times
	float root = sqrtf(underRoot);
	float rootPlus = -rayDiffProj + root;
	float rootMinus = -rayDiffProj - root;
	return rootPlus > rootMinus ? rootMinus : rootPlus;
}

Vector3 Sphere::getIntersetctionPoint(Ray ray)
{
	float d = findIntersection(ray);
	return d * ray.direction;
}