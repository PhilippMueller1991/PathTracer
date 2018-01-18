#pragma once

#include "Transform.h"
#include "Ray.h"
#include "Material.h"

class SceneObject
{
public:
	Transform transform;
	Material material;

	SceneObject(Transform t = Transform(), Material m = Material(1, 2)) 
		: transform(t), material(m)
	{
	}
	~SceneObject() {}

	virtual Vector3 getNormalAt(Vector3 pos) = 0;
	virtual Color getColorAt(Vector3 pos) = 0;
	virtual float findIntersection(Ray ray) = 0;
};

