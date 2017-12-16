#pragma once

#include "Vector3.h"
#include "Camera.h"
#include "Ray.h"
#include "Intersection.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Scene.h"
#include "Image.h"

#include <iostream>
#include <vector>
#include <time.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>

#define PI static_cast<float>(M_PI)

class Raytracer
{
public:
	static int maxBounces;
	static int samplesPerPixel;
	Scene* scene;

	Raytracer(Scene* scene) : scene(scene)
	{
	}
	~Raytracer() {}

	Intersection ComputeFirstRayObjectIntersection(const Ray& ray);
	Color EvaluateLocalLightingModel(const Vector3& hitPos, const Vector3& normal, const Material& mat);
	Color PhongLightingModel(const Vector3& hitPos, const Vector3& normal, const Material& mat, const Light& light);
	bool IsInShadow(const Vector3& hitPos, const Light& light);
	Color Traverse(const Ray& ray);
	void Render(int width, int height);
};