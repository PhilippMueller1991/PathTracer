#pragma once

#include "SceneObject.h"

class Sphere : public SceneObject
{
public:
	//Vector pos;
	float r;

	// SceneObject constructor with empty parameters is called automatically setting color
	Sphere() :  r(1.0f) 
	{
		transform.pos = Vec3f(0, 0, 0);
	}
	Sphere(Vec3f pos, float radius = 1.0f, Material m = Material(1, 2))
		: r(radius), SceneObject(Transform(pos), m) 
	{
	}

	Vec3f getNormalAt(Vec3f pos) override;
	Color getColorAt(Vec3f pos) override { return material.getColorAt(0.0f, 0.0f); }	// TODO
	bool findIntersection(Ray& ray, float& dist) override;
};

