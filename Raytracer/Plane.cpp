#include "Plane.h"

Vector3 Plane::getNormalAt(Vector3 pos)
{
	return normal;
}

Vector3 Plane::getIntersetctionPoint(Ray ray)
{
	float d = findIntersection(ray);
	return d * ray.direction;
}

float Plane::findIntersection(Ray ray)
{
	float denominator = ray.direction.Dot(normal);
	float enumerator = (pos - ray.origin).Dot(normal);
	//float enumerator = -(ray.origin.Dot(normal) + pos.Magnitude());

	if (denominator == 0 || enumerator == 0)
	{
		return -1;
	}
	else
	{
		return enumerator/denominator;
	}
}