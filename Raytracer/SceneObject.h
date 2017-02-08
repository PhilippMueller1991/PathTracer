#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

class SceneObject
{
public:
	Material material;

	SceneObject() : material(1,2,1) {}
	SceneObject(Material m) : material(m) {}
	~SceneObject() {}

	virtual Vector3 getNormalAt(Vector3 pos) = 0;

	virtual Vector3 getIntersetctionPoint(Ray ray) = 0;
	virtual float findIntersection(Ray ray) = 0;
};

