#include "Raytracer.h"

#include <omp.h>

#define NUM_THREADS 8

int Raytracer::maxBounces;
int Raytracer::samplesPerPixel;

const float ambientIntensity = 0.0f;
const Color ambientColor = ambientIntensity * Color(0.3f, 0.3f, 0.5f);
const float specularExp = 80.0f;

Intersection Raytracer::ComputeFirstRayObjectIntersection(Ray& ray)
{
	Intersection nearestIntersection(-1, FLT_MAX);
	for (uint32_t i = 0; i < scene->objects.size(); i++)
	{
		float intersectionDistance = -1.0f; 
		if (scene->objects[i]->FindIntersection(ray, intersectionDistance) &&
			intersectionDistance > 0 && intersectionDistance < nearestIntersection.distance)
		{
			nearestIntersection.distance = intersectionDistance;
			nearestIntersection.idx = i;
		}
	}
	return nearestIntersection;
}

Color Raytracer::EvaluateLocalLightingModel(const Vec3f& hitPos, const Vec3f& normal, const Material& mat, const Color& texCol)
{
	Color c = ambientColor;
	Color tmp;
	for (uint32_t i = 0; i < scene->lights.size(); i++)
	{
		tmp = PhongLightingModel(hitPos, normal, mat, texCol, *scene->lights[i]);
		c.r += tmp.r;
		c.g += tmp.g;
		c.b += tmp.b;
	}
	return c / static_cast<float>(scene->lights.size());
}

Color Raytracer::PhongLightingModel(const Vec3f& hitPos, const Vec3f& normal, const Material& mat, const Color& texCol, const Light& light)
{
	// Early exit for normals that point away from current light and camera
	// Camera cull allowed because we only evaluate the lighting model for diffuse surfaces
	bool isBackwardsNormal = normal.Dot(light.pos - hitPos) < 0;
	bool isInvisibleForCam = normal.Dot(scene->cam.pos - hitPos) < 0;
	float shadowPercentage = IsInShadow(hitPos, light);
	if (isBackwardsNormal || isInvisibleForCam || shadowPercentage >= 1.0f)
		return ambientColor;

	// Phong lighnting model with energy conservation in specular part
	Vec3f lightDir = (light.pos - hitPos).Normalize();
	Vec3f viewDir = (scene->cam.pos - hitPos).Normalize();
	Vec3f reflectDir = Vec3f::Reflect(-lightDir, normal);
	reflectDir = mat.DisturbeReflectionDir(reflectDir);

	Color diffuseColor = fmaxf(0.0f, lightDir.Dot(normal)) * texCol;
	Color specularColor = (specularExp + 1.0f) / (2.0f * PI) * powf(fmaxf(0.0f, reflectDir.Dot(viewDir)), specularExp) * mat.specularColor;

	Color c = light.color * (diffuseColor + specularColor);
	c *= light.intensity;
	return (1.0f - shadowPercentage) * c + shadowPercentage * ambientColor;
}

// TODO: Look at papers how they handle transparent shadows
float Raytracer::IsInShadow(const Vec3f& hitPos, const Light& light)
{
	Vec3f rayDir = (light.pos - hitPos).Normalize();
	Ray shadowRay(hitPos, rayDir);
	Intersection intersection = ComputeFirstRayObjectIntersection(shadowRay);

	// Ray never intersected with geometry
	if (intersection.idx < 0)
		return 0.0f;

	// Transparent objects don't block light fully
	float kt = scene->objects[intersection.idx]->material.GetKt();
	if (kt > EPS)
	{	
		//float tmp = IsInShadow(intersection.distance * rayDir, light);
		//return fminf(1.0f, fmaxf(1.0f, 1.0f - kt + tmp));
		return fmaxf(0.0f, 1.0f - kt/1.1f);
	}

	// Commented out calculations differs in range 10e-7
	float lightDistance = (light.pos - hitPos).Dot(rayDir);	//(light.pos - hitPos).Magnitude();

	// Test if anything blocks the light
	if (intersection.distance < lightDistance)
		return 1.0f;
	else
		return 0.0f;
}

Color Raytracer::Traverse(Ray& ray)
{
	Color color = Color::black;

	// Termination criteria
	if (ray.bounce > maxBounces)
		return color;

	Intersection intersection = ComputeFirstRayObjectIntersection(ray);
	// No object was hit, return background color
	if (intersection.idx < 0)
		return color;

	Material mat = scene->objects[intersection.idx]->material;
	Vec3f hitPos = ray.direction * intersection.distance + ray.origin;
	Vec3f normal = scene->objects[intersection.idx]->GetNormalAt(hitPos);
	Color texColor = scene->objects[intersection.idx]->GetColorAt(hitPos);

	color = EvaluateLocalLightingModel(hitPos, normal, mat, texColor);

	// Early return for purely diffuse surfaces
	if (mat.GetKs() < EPS && mat.GetKt() < EPS)
		return color;

	// Reflection
	Color reflectionColor;
	if (mat.GetKs() > EPS)
	{
		Vec3f reflectDir = Vec3f::Reflect(ray.direction, normal);
		Ray reflectRay = Ray(hitPos, reflectDir, ray.bounce + 1, mat.refractiveIndex);
		reflectRay.direction = mat.DisturbeReflectionDir(reflectRay.direction);
		reflectionColor = mat.GetKs() * Traverse(reflectRay);
	}

	// Transmission and refraction
	Color transmissionColor;
	if (mat.GetKt() > EPS)
	{
		float idr0 = ray.lastIDR;
		float idr1 = mat.refractiveIndex;

		// Total Internal Refelction
		// Reflection percentage
		float R = Vec3f::FresnelReflectance(ray.direction, normal, idr0, idr1);	
		// Transmission percentage
		float T = 1.0f - R;	
		if (T > EPS)
		{
			Vec3f transmissionDir = Vec3f::Refract(ray.direction, normal, idr0, idr1);
			Ray transmissionRay = Ray(hitPos + transmissionDir * EPS, transmissionDir, ray.bounce + 1, mat.refractiveIndex);
			transmissionRay.direction = mat.DisturbeReflectionDir(transmissionDir);
			transmissionColor += T * mat.GetKt() * Traverse(transmissionRay);
		}
		if (R > EPS)
		{
			Vec3f reflectDir = Vec3f::Reflect(ray.direction, normal);
			Ray reflectRay = Ray(hitPos, reflectDir, ray.bounce + 1, mat.refractiveIndex);
			reflectRay.direction = mat.DisturbeReflectionDir(reflectRay.direction);
			transmissionColor += R * mat.GetKs() * Traverse(reflectRay);
		}
	}

	return color + reflectionColor + transmissionColor;
}

// TODO: Choose  better jitter for random samples per pixel (Poison jitter, Sobol jitter, ...)
// TODO: Use space partitioning for faster raytracing (ray-object intersection)
void Raytracer::Render(int width, int height)
{
	Camera& cam = scene->cam;
	Image img(width, height);
	RGB* pixels = img.data;

	// OMP settings
	omp_set_num_threads(NUM_THREADS);	// Number of logical CPU kernels

	int progress = 0;
	// Render scene to image
	for (int y = 0; y < img.height; y++)
	{
		#pragma omp parallel for //schedule(dynamic) //nowait
		for (int x = 0; x < img.width; x++)
		{
			int idx = y * img.width + x;
			Ray camRay(cam.pos, cam.PixelToRayDir(x, y));
			Color color = Traverse(camRay);
			float xOffset, yOffset;	// Must be in range(-0.5,0.5)
			//#pragma omp parallel for reduction(+:color.r, +:color.g, +:color.b)
			for (int s = 1; s < samplesPerPixel; s++)
			{
				// Simple random sampling, better sampling methods achieve better results 
				xOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				yOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				camRay = Ray(cam.pos, cam.PixelToRayDir(x, y, xOffset, yOffset));
				Color tmp = Traverse(camRay);
				color.r += tmp.r;
				color.g += tmp.g;
				color.b += tmp.b;
			}
			// Normalize color
			color /= static_cast<float>(samplesPerPixel);
			img.SetPixel(idx, color);	// False sharing could occur for multiple threads
		}

		// Print progress in percent to console
		int nextProgress = (int)(100.0f * y / (float)img.height);
		if (nextProgress > progress)
		{
			progress = nextProgress;
			std::cout << "Progress: " << progress << "%" << std::endl;
		}
	}

	std::cout << "Writing scene.bmp" << std::endl;
	img.SaveBitmap("..//scene.bmp");
}