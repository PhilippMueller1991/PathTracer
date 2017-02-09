#pragma once

#include "Vector3.h"
#include "Camera.h"
#include "Ray.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Scene.h"

#include "Image.h"

#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>

class Raytracer
{
public:
	struct Intersection
	{
		int idx;
		float distance;

		Intersection(int idx, float distance) : idx(idx), distance(distance) {}
	};

	static int maxBounces;
	Scene* scene;

	Raytracer() 
	{

	}
	~Raytracer() {}

	Intersection ComputeFirstRayObjectIntersection(const Ray& ray);
	Color EvaluateLocalLightingModel(const Vector3& normal, const Material& mat, const Light& light);
	bool IsInShadow(const Vector3& hitPos, const Light& light);
	Color Traverse(const Ray& ray);
	void Render(int width, int height);
};