#pragma once

#include "Transform.h"
#include "Ray.h"
#include "Material.h"

// Abstract class
class SceneObject
{
public:
	Transform transform = Transform();
	Material material = Material(1,2);
public:
	SceneObject(Transform t = Transform(), Material m = Material(1, 2)) 
		: transform(t), material(m)
	{
	}

	virtual Vec3f getNormalAt(Vec3f pos) = 0;
	virtual Color getColorAt(Vec3f pos) = 0;
	virtual bool findIntersection(Ray& ray, float& dist) = 0;
};

