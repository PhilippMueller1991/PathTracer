#include "Plane.h"
#include <math.h>

Vec3f Plane::getNormalAt(Vec3f pos)
{
	return normal;
}

bool Plane::findIntersection(Ray& ray, float& dist)
{
	float denominator = ray.direction.dot(normal);
	float enumerator = (transform.pos - ray.origin).dot(normal);

	if (fabsf(denominator) <= EPS || fabsf(enumerator) <= EPS)
		return false;
	
	float d = enumerator / denominator;
	Vec3f hitPos = ray.origin + ray.direction * d;
	Vec3f xDir = transform.rot * Vec3f::right;
	Vec3f yDir = transform.rot * Vec3f::up;
	float xDistance = hitPos.dot(xDir);
	float yDistance = hitPos.dot(yDir);

	if (fabsf(xDistance) > transform.scale.x / 2.0f || fabsf(yDistance) > transform.scale.y / 2.0f)
		return false;

	dist = d;
	return true;
}

Color Plane::getColorAt(Vec3f pos)
{
	if (material.texture == nullptr)
		return material.diffuseColor;

	Vec3f xDir = transform.rot * Vec3f::right;
	Vec3f yDir = transform.rot * Vec3f::up;
	float xDistance = pos.dot(xDir);
	float yDistance = pos.dot(yDir);

	float u = (xDistance + transform.scale.x / 2.0f) / (transform.scale.x);
	float v = (yDistance + transform.scale.y / 2.0f) / (transform.scale.y);

	return material.getColorAt(u, v);
}