#pragma once

#include "SceneObject.h"

class Plane : public SceneObject
{
public:
	Vector3 pos;
	Vector3 normal;

	Plane() : pos(Vector3(0,0,0)), normal(-static_cast<Vector3>(Vector3::forward)) {}
	Plane(Vector3 pos, Vector3 normal, Material m = Material(1, 2)) : pos(pos), normal(normal), SceneObject(m) {}
	~Plane() {}

	Vector3 getNormalAt(Vector3 pos);
	float findIntersection(Ray ray);
};

