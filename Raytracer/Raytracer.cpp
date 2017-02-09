#include "Raytracer.h"

#define _USE_MATH_DEFINES
#include <math.h>

int Raytracer::maxBounces = 1;

const float ambientIntensity = 0.05f;
const Color ambientColor = ambientIntensity * Color(0.5f, 0.5f, 1);

Raytracer::Intersection Raytracer::ComputeFirstRayObjectIntersection(const Ray& ray)
{
	Intersection nearestIntersection(-1, FLT_MAX);
	for (uint32_t i = 0; i < scene->objects.size(); i++)
	{
		float intersectionDistance = scene->objects[i]->findIntersection(ray);
		if (intersectionDistance > 0 && intersectionDistance < nearestIntersection.distance)
		{
			nearestIntersection.distance = intersectionDistance;
			nearestIntersection.idx = i;
		}
	}

	return nearestIntersection;
}

Color Raytracer::EvaluateLocalLightingModel(const Vector3& hitPos, const Vector3& normal, const Material& mat, const Light& light)
{
	// Early exit for normals that point away from current light
	bool backwardsNormal = normal.Dot(light.pos - hitPos) < 0;
	if (backwardsNormal || IsInShadow(hitPos, light))
		return ambientColor;

	// Phong lighnting model with energy conservation in specular part
	Vector3 l = (light.pos - hitPos).Normalize();
	Vector3 v = (scene->cam.pos - hitPos).Normalize();
	Vector3 r = Vector3::Reflect(l, normal).Normalize();

	Color diffuseColor = fmaxf(0.0f, l.Dot(normal)) * light.color;
	Color specularColor = (mat.specularExp + 1) / (2.0f * M_PI) * powf(fmaxf(0.0f, r.Dot(v)), mat.specularExp) * mat.specularColor;
	Color lightning = ambientColor + diffuseColor;

	Color color = mat.diffuseColor;
	color *= lightning;
	color += specularColor;
	return color;
}

// TODO: IsInShadow produces much noise for sphere intersections
bool Raytracer::IsInShadow(const Vector3& hitPos, const Light& light)
{
	Vector3 rayDir = (light.pos - hitPos).Normalize();
	Ray shadowRay(hitPos, rayDir, Ray::RAY_SHADOW, 1);
	Intersection intersection = ComputeFirstRayObjectIntersection(shadowRay);

	// Calculations differs in range 10e-7
	float lightDistance = (light.pos - hitPos).Dot(rayDir);	//(light.pos - hitPos).Magnitude();

	if (intersection.idx < 0 || intersection.distance > lightDistance)
		return false;

	return true;
}

Color Raytracer::Traverse(const Ray& ray)
{
	Color color = Color::black;
	Intersection intersection = ComputeFirstRayObjectIntersection(ray);
	
	// No object was hit, return background color
	if (intersection.idx < 0)
		return color;

	// Pathtracer: Randomly choose one of the following rays: 
	// { normal, reflection, refraction }
	Vector3 hitPos = ray.direction * intersection.distance + ray.origin;
	Vector3 normal = scene->objects[intersection.idx]->getNormalAt(hitPos);

	// For now only test for one light
	const Light& light = *scene->lights[0];
	color = EvaluateLocalLightingModel(hitPos, normal, scene->objects[intersection.idx]->material, light);

	// Pathtracer: Needs additional material property diffuse in recursive call
	//Material mat;
	//return color + mat.ks * Traverse(reflect) + mat.kt * Traverse(refract);
	return color.Clamp();
}

// TODO: Fix render code for non square images
void Raytracer::Render(int width, int height)
{
	Camera& cam = scene->cam;
	Image img(width, height);
	RGB* pixels = img.data;
	for (int y = 0; y < img.height; y++)
	{
		for (int x = 0; x < img.width; x++)
		{
			int idx = y * img.width + x;
			// TODO: Accomodate for aspectRatio
			// TODO: Shoot multiple rays with random jitter (later Sobol jitter, or Multi-Jittered) for AA
			Ray camRay(cam.pos, cam.PixelToRayDir(x, y, img.width, img.width));
			Color color = Traverse(camRay);
			img.SetPixel(idx, color);
		}
	}

	std::cout << "Writing scene.bmp" << std::endl;
	img.SaveBitmap("..//scene.bmp");
}