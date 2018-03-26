#pragma once

#include "Vector.h"
#include "Camera.h"
#include "Ray.h"
#include "Intersection.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Mesh.h"
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

	Intersection ComputeFirstRayObjectIntersection(Ray& ray);
	Color EvaluateLocalLightingModel(const Vec3f& hitPos, const Vec3f& normal, const Material& mat, const Color& texCol);
	Color PhongLightingModel(const Vec3f& hitPos, const Vec3f& normal, const Material& mat, const Color& texCol, const Light& light);
	float IsInShadow(const Vec3f& hitPos, const Light& light);
	Color Traverse(Ray& ray);
	void Render(int width, int height);
};