#include "Raytracer.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>

#define PI static_cast<float>(M_PI)

int Raytracer::maxBounces;
int Raytracer::samplesPerPixel;

const float ambientIntensity = 0.0f;
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

Color Raytracer::EvaluateLocalLightingModel(const Vector3& hitPos, const Vector3& normal, const Material& mat)
{
	Color c(0,0,0);
	for (uint32_t i = 0; i < scene->lights.size(); i++)
	{
		c += PhongLightingModel(hitPos, normal, mat, *scene->lights[i]);
		//c += MicroFacetLightingModel(hitPos, normal, mat, *scene->lights[i]);
	}
	return c;
}

Color Raytracer::PhongLightingModel(const Vector3& hitPos, const Vector3& normal, const Material& mat, const Light& light)
{
	// Early exit for normals that point away from current light
	bool backwardsNormal = normal.Dot(light.pos - hitPos) < 0;
	if (backwardsNormal || IsInShadow(hitPos, light))
		return ambientColor;

	// Phong lighnting model with energy conservation in specular part
	Vector3 lightDir = (light.pos - hitPos).Normalize();
	Vector3 viewDir = (scene->cam.pos - hitPos).Normalize();
	Vector3 reflectDir = Vector3::Reflect(-lightDir, normal);

	Color diffuseColor = fmaxf(0.0f, lightDir.Dot(normal)) * light.color;
	Color specularColor = (mat.specularExp + 1) / (2.0f * PI) * powf(fmaxf(0.0f, reflectDir.Dot(viewDir)), mat.specularExp) * mat.specularColor;

	return mat.diffuseColor * (ambientColor + diffuseColor) + specularColor;
}

// TODO
Color Raytracer::MicroFacetLightingModel(const Vector3& hitPos, const Vector3& normal, const Material& mat, const Light& light)
{
	// Early exit for normals that point away from current light
	bool backwardsNormal = normal.Dot(light.pos - hitPos) < 0;
	if (backwardsNormal || IsInShadow(hitPos, light))
		return ambientColor;

	// For now return flat diffuse color
	return mat.diffuseColor * light.color;
}

bool Raytracer::IsInShadow(const Vector3& hitPos, const Light& light)
{
	Vector3 rayDir = (light.pos - hitPos).Normalize();
	Ray shadowRay(hitPos, rayDir);
	Intersection intersection = ComputeFirstRayObjectIntersection(shadowRay);

	// Calculations differs in range 10e-7
	float lightDistance = (light.pos - hitPos).Dot(rayDir);	//(light.pos - hitPos).Magnitude();

	if (intersection.idx < 0 || intersection.distance > lightDistance)
		return false;

	return true;
}

// TODO: Use frasnel refraction formular
// TODO: Fix transmission (probably object leaving wrong)
// TBD: Do we need the EPS vector offset in transmission?
Color Raytracer::Traverse(const Ray& ray)
{
	Color color = Color::black;

	// Termination criteria
	if (ray.bounce > maxBounces) 
		return color;

	Intersection intersection = ComputeFirstRayObjectIntersection(ray);
	// No object was hit, return background color
	if (intersection.idx < 0)
		return color;

	Vector3 hitPos = ray.direction * intersection.distance + ray.origin;
	Vector3 normal = scene->objects[intersection.idx]->getNormalAt(hitPos);
	Material mat = scene->objects[intersection.idx]->material;

	color = EvaluateLocalLightingModel(hitPos, normal, mat);

	// Reflection
	Color reflectionColor;
	Vector3 reflectDir = Vector3::Reflect(ray.direction, normal);
	Ray reflectRay = Ray(hitPos, reflectDir, ray.bounce + 1);
	if(mat.GetKs() > 0)
		reflectionColor = mat.GetKs() * Traverse(reflectRay);

	// Transmission / Refraction
	Color transmissionColor;
	if (mat.GetKt() > 0)
	{
		bool leavingObject = normal.Dot(ray.direction) > 0;
		float n1 = leavingObject ? mat.refractiveIndex : 1.0f;
		float n2 = leavingObject ? 1.0f : mat.refractiveIndex;
		Vector3 transmissionDir = Vector3::Refract(ray.direction, normal, n1, n2);
		if (transmissionDir != Vector3::invalid)
		{
			Ray transmissionRay = Ray(hitPos + transmissionDir * EPS, transmissionDir, ray.bounce + 1);
			transmissionColor = mat.GetKt() * Traverse(transmissionRay);
		}
	}

	return (color + reflectionColor + transmissionColor).Clamp();
}

// TODO: Fix render code for non square images, Accomodate for aspectRatio
// TODO: Shoot multiple rays with random jitter (later Sobol jitter, or Multi-Jittered) for AA
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
			Color color;
			Ray camRay(cam.pos, cam.PixelToRayDir(x, y));
			color += Traverse(camRay);
			float xOffset, yOffset;	// Must be in range(-0.5,0.5)
			for (int s = 1; s < samplesPerPixel; s++)
			{
				xOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				yOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				camRay = Ray(cam.pos, cam.PixelToRayDir(x, y, xOffset, yOffset));
				color += Traverse(camRay);
			}
			color /= samplesPerPixel;
			img.SetPixel(idx, color);
		}
	}

	std::cout << "Writing scene.bmp" << std::endl;
	img.SaveBitmap("..//scene.bmp");
}