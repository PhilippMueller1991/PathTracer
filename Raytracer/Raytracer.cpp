#include "Raytracer.h"

#include <omp.h>

#define NUM_THREADS 8

int Raytracer::maxBounces;
int Raytracer::samplesPerPixel;

const float ambientIntensity = 0.0f;
const Color ambientColor = ambientIntensity * Color(0.3f, 0.3f, 0.5f);
const float specularExp = 80.0f;

Intersection Raytracer::computeFirstRayObjectIntersection(Ray& ray)
{
	Intersection nearestIntersection(-1, FLT_MAX);
	for (uint32_t i = 0; i < scene->objects.size(); i++)
	{
		float intersectionDistance = -1.0f; 
		if (scene->objects[i]->findIntersection(ray, intersectionDistance) &&
			intersectionDistance > 0 && intersectionDistance < nearestIntersection.distance)
		{
			nearestIntersection.distance = intersectionDistance;
			nearestIntersection.idx = i;
		}
	}
	return nearestIntersection;
}

Color Raytracer::evaluateLocalLightingModel(const Vec3f& hitPos, const Vec3f& normal, const Material& mat, const Color& texCol)
{
	Color c = ambientColor;
	Color tmp;
	for (uint32_t i = 0; i < scene->lights.size(); i++)
	{
		tmp = phongLightingModel(hitPos, normal, mat, texCol, *scene->lights[i]);
		c.r += tmp.r;
		c.g += tmp.g;
		c.b += tmp.b;
	}
	return c / static_cast<float>(scene->lights.size());
}

Color Raytracer::phongLightingModel(const Vec3f& hitPos, const Vec3f& normal, const Material& mat, const Color& texCol, const Light& light)
{
	// Early exit for normals that point away from current light and camera
	// Camera cull allowed because we only evaluate the lighting model for _diffuse_ surfaces
	bool isBackwardsNormal = normal.dot(light.pos - hitPos) < 0;
	bool isInvisibleForCam = normal.dot(scene->cam.pos - hitPos) < 0;
	float shadowPercentage = 0.0f; //isInShadow(hitPos, light);
	if (isBackwardsNormal || isInvisibleForCam || shadowPercentage >= 1.0f)
		return ambientColor;

	// Phong lighnting model with energy conservation in specular part
	Vec3f lightDir = (light.pos - hitPos).normalize();
	Vec3f viewDir = (scene->cam.pos - hitPos).normalize();
	Vec3f reflectDir = Vec3f::reflect(-lightDir, normal);
	reflectDir = mat.DisturbeReflectionDir(reflectDir);

	Color diffuseColor = fmaxf(0.0f, lightDir.dot(normal)) * texCol;
	Color specularColor = (specularExp + 1.0f) / (2.0f * PI) * powf(fmaxf(0.0f, reflectDir.dot(viewDir)), specularExp) * mat.specularColor;

	Color c = light.color * (diffuseColor + specularColor);
	c *= light.intensity;
	return (1.0f - shadowPercentage) * c + shadowPercentage * ambientColor;
}

// TODO: Look at papers how they handle transparent shadows
float Raytracer::isInShadow(const Vec3f& hitPos, const Light& light)
{
	Vec3f rayDir = (light.pos - hitPos).normalize();
	Ray shadowRay(hitPos, rayDir);
	Intersection intersection = computeFirstRayObjectIntersection(shadowRay);

	// Ray never intersected with geometry
	if (intersection.idx < 0)
		return 0.0f;

	// Transparent objects don't block light fully
	float kt = scene->objects[intersection.idx]->material.getKt();
	if (kt > EPS)
	{	
		//float tmp = IsInShadow(intersection.distance * rayDir, light);
		//return fminf(1.0f, fmaxf(1.0f, 1.0f - kt + tmp));
		return fmaxf(0.0f, 1.0f - kt/1.1f);
	}

	// Commented out calculations differs in range 10e-7
	float lightDistance = (light.pos - hitPos).dot(rayDir);	//(light.pos - hitPos).Magnitude();

	// Test if anything blocks the light
	if (intersection.distance < lightDistance)
		return 1.0f;
	else
		return 0.0f;
}

Color Raytracer::traverse(Ray& ray)
{
	Color color = Color::black;

	// Termination criteria
	if (ray.bounce > maxBounces)
		return color;

	Intersection intersection = computeFirstRayObjectIntersection(ray);
	// No object was hit, return background color
	if (intersection.idx < 0)
		return color;

	Material mat = scene->objects[intersection.idx]->material;
	Vec3f hitPos = ray.direction * intersection.distance + ray.origin;
	Vec3f normal = scene->objects[intersection.idx]->getNormalAt(hitPos);
	Color texColor = scene->objects[intersection.idx]->getColorAt(hitPos);

	color = evaluateLocalLightingModel(hitPos, normal, mat, texColor);

	// Early return for purely diffuse surfaces
	if (mat.getKs() < EPS && mat.getKt() < EPS)
		return color;

	// Reflection
	Color reflectionColor;
	if (mat.getKs() > EPS)
	{
		Vec3f reflectDir = Vec3f::reflect(ray.direction, normal);
		Ray reflectRay = Ray(hitPos, reflectDir, ray.bounce + 1, mat.refractiveIndex);
		reflectRay.direction = mat.DisturbeReflectionDir(reflectRay.direction);
		reflectionColor = mat.getKs() * traverse(reflectRay);
	}

	// Transmission / Refraction
	Color transmissionColor;
	if (mat.getKt() > EPS)
	{
		float idr0 = ray.lastIDR;
		float idr1 = mat.refractiveIndex;

		// Total Internal Refelction (TIR)
		float R = Vec3f::fresnelReflectance(ray.direction, normal, idr0, idr1);	// Reflection percentage
		float T = 1.0f - R;	// Transmission percentage
		if (T > EPS)
		{
			// TBD: Do we need the EPS vector offset in transmission?
			Vec3f transmissionDir = Vec3f::refract(ray.direction, normal, idr0, idr1);
			Ray transmissionRay = Ray(hitPos + transmissionDir * EPS, transmissionDir, ray.bounce + 1, mat.refractiveIndex);
			transmissionRay.direction = mat.DisturbeReflectionDir(transmissionDir);
			transmissionColor += T * mat.getKt() * traverse(transmissionRay);
		}
		if (R > EPS)
		{
			Vec3f reflectDir = Vec3f::reflect(ray.direction, normal);
			Ray reflectRay = Ray(hitPos, reflectDir, ray.bounce + 1, mat.refractiveIndex);
			reflectRay.direction = mat.DisturbeReflectionDir(reflectRay.direction);
			transmissionColor += R * mat.getKs() * traverse(reflectRay);
		}
	}

	return color + reflectionColor + transmissionColor;
}

// TODO: Choose  better jitter for random samples per pixel (Poison jitter, Sobol jitter, ...)
// TODO: Use space partitioning for faster raytracing (ray-object intersection)
void Raytracer::render(int width, int height)
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
			Ray camRay(cam.pos, cam.pixelToRayDir(x, y));
			Color color = traverse(camRay);
			float xOffset, yOffset;	// Must be in range(-0.5,0.5)
			//#pragma omp parallel for reduction(+:color.r, +:color.g, +:color.b)
			for (int s = 1; s < samplesPerPixel; s++)
			{
				// Simple random sampling, better sampling methods achieve better results 
				xOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				yOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				camRay = Ray(cam.pos, cam.pixelToRayDir(x, y, xOffset, yOffset));
				Color tmp = traverse(camRay);
				color.r += tmp.r;
				color.g += tmp.g;
				color.b += tmp.b;
			}
			// Normalize color
			color /= static_cast<float>(samplesPerPixel);
			img.setPixel(idx, color);	// False sharing could occur for multiple threads
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
	img.saveBitmap("..//scene.bmp");
}