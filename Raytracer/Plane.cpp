#include "Plane.h"
#include <math.h>

Vector3 Plane::getNormalAt(Vector3 pos)
{
	return normal;
}

float Plane::findIntersection(Ray ray)
{
	float denominator = ray.direction.Dot(normal);
	float enumerator = (pos - ray.origin).Dot(normal);

	if (fabsf(denominator) <= EPS || fabsf(enumerator) <= EPS)
	{
		return -1;
	}
	else
	{
		return enumerator/denominator;
	}
}