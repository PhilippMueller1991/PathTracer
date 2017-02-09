#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

class SceneObject
{
public:
	Material material;

	SceneObject() : material(1,2) {}
	SceneObject(Material m) : material(m) {}
	~SceneObject() {}

	virtual Vector3 getNormalAt(Vector3 pos) = 0;
	virtual float findIntersection(Ray ray) = 0;
};

