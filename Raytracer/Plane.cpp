#include "Plane.h"
#include <math.h>

Vector3 Plane::getNormalAt(Vector3 pos)
{
	return normal;
}

// TODO: Fix for normals that are +/-Vector3::up
float Plane::findIntersection(Ray ray)
{
	float denominator = ray.direction.Dot(normal);
	float enumerator = (transform.pos - ray.origin).Dot(normal);

	if (fabsf(denominator) <= EPS || fabsf(enumerator) <= EPS)
	{
		return -1;
	}
	
	const float d = enumerator / denominator;
	const Vector3 hitPos = ray.origin + ray.direction * d;
	const Vector3 xDir = transform.rot * Vector3::right;
	const Vector3 yDir = transform.rot * Vector3::up;
	const float xDistance = hitPos.Dot(xDir);
	const float yDistance = hitPos.Dot(yDir);

	if (fabsf(xDistance) > transform.scale.x / 2.0f || fabsf(yDistance) > transform.scale.y / 2.0f)
		return -1;

	return d;
}