#pragma once

#include "SceneObject.h"

class Plane : public SceneObject
{
protected:
	Vec3f normal;

public:
	Plane()
		: SceneObject()
	{
		normal = Vec3f::forward;
	}
	Plane(Vec3f pos, Rotation rot, Vec3f scale, Material m = Material(1, 2))
		: SceneObject(Transform(pos,rot,scale), m)
	{
		normal = rot * Vec3f::forward;
	}

	Vec3f GetNormalAt(Vec3f pos) override;
	Color GetColorAt(Vec3f pos) override;
	bool FindIntersection(Ray& ray, float& dist) override;
};

