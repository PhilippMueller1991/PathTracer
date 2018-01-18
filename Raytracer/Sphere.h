#pragma once

#include "SceneObject.h"

class Sphere : public SceneObject
{
public:
	//Vector3 pos;
	float r;

	// SceneObject constructor with empty parameters is called automatically setting color
	Sphere() :  r(1.0f) 
	{
		transform.pos = Vector3(0, 0, 0);
	}
	Sphere(Vector3 pos, float radius = 1.0f, Material m = Material(1, 2)) 
		: r(radius), SceneObject(Transform(pos), m) 
	{
	}
	~Sphere() {}

	Vector3 getNormalAt(Vector3 pos) override;
	Color getColorAt(Vector3 pos) override { return material.GetColorAt(0.0f, 0.0f); }	// TODO
	float findIntersection(Ray ray) override;
};

