#pragma once

#include "SceneObject.h"

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

	Vector3 getNormalAt(Vector3 pos) override;
	Color getColorAt(Vector3 pos) override;
	float findIntersection(Ray ray) override;
};

