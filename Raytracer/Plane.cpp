#include "Plane.h"
#include <math.h>

Vector3 Plane::getNormalAt(Vector3 pos)
{
	return normal;
}

float Plane::findIntersection(Ray ray)
{
	float denominator = ray.direction.Dot(normal);
	float enumerator = (transform.pos - ray.origin).Dot(normal);

	if (fabsf(denominator) <= EPS || fabsf(enumerator) <= EPS)
	{
		return -1;
	}
	
	float d = enumerator / denominator;
	Vector3 hitPos = ray.origin + ray.direction * d;
	Vector3 xDir = transform.rot * Vector3::right;
	Vector3 yDir = transform.rot * Vector3::up;
	float xDistance = hitPos.Dot(xDir);
	float yDistance = hitPos.Dot(yDir);

	if (fabsf(xDistance) > transform.scale.x / 2.0f || fabsf(yDistance) > transform.scale.y / 2.0f)
		return -1;

	return d;
}

Color Plane::getColorAt(Vector3 pos)
{
	if (material.texture == nullptr)
		return material.diffuseColor;

	Vector3 xDir = transform.rot * Vector3::right;
	Vector3 yDir = transform.rot * Vector3::up;
	float xDistance = fabsf(pos.Dot(xDir));
	float yDistance = fabsf(pos.Dot(yDir));

	float u = xDistance / (transform.scale.x / 2.0f);
	float v = yDistance / (transform.scale.y / 2.0f);

	return material.GetColorAt(u, v);
}