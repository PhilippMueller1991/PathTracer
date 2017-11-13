#pragma once

#include "SceneObject.h"

// TODO: Set normal from rotation
class Plane : public SceneObject
{
protected:
	Vector3 normal;

public:
	Plane()
		: SceneObject()
	{
		normal = Vector3::forward;
	}
	Plane(Vector3 pos, Rotation rot, Vector3 scale, Material m = Material(1, 2))
		: SceneObject(Transform(pos,rot,scale), m)
	{
		normal = rot * Vector3::forward;
	}
	~Plane() {}

	Vector3 getNormalAt(Vector3 pos);
	float findIntersection(Ray ray);
};

