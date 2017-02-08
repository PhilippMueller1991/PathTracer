#pragma once

#include "SceneObject.h"

class Sphere : public SceneObject
{
public:
	Vector3 pos;
	float r;

	// SceneObject constructor with empty parameters is called automatically setting color
	Sphere() : pos(Vector3(0,0,0)), r(1.0f) { }
	Sphere(Vector3 position, float radius = 1.0f, Material m = Material(1, 2, 1)) : pos(position), r(radius), SceneObject(m) {}
	~Sphere() {}

	Vector3 getNormalAt(Vector3 pos);

	float findIntersection(Ray ray);
	Vector3 getIntersetctionPoint(Ray ray);
};

