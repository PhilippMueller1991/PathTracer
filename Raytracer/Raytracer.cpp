#include "Raytracer.h"

#include <omp.h>

int Raytracer::maxBounces;
int Raytracer::samplesPerPixel;

const float ambientIntensity = 0.0f;
const Color ambientColor = ambientIntensity * Color(0.5f, 0.5f, 0.5f);
const float specularExp = 80.0f;

Intersection Raytracer::ComputeFirstRayObjectIntersection(const Ray& ray)
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
	Color c(0, 0, 0);
	for (uint32_t i = 0; i < scene->lights.size(); i++)
		c += PhongLightingModel(hitPos, normal, mat, *scene->lights[i]);
	return c;
}

// TODO: Use textures
// TODO: Fix shadows for transparent objects
Color Raytracer::PhongLightingModel(const Vector3& hitPos, const Vector3& normal, const Material& mat, const Light& light)
{
	// Early exit for normals that point away from current light
	const bool backwardsNormal = normal.Dot(light.pos - hitPos) < 0;
	if (backwardsNormal || (mat.GetKt() < EPS && IsInShadow(hitPos, light)))
		return ambientColor;

	// Phong lighnting model with energy conservation in specular part
	Vector3 lightDir = (light.pos - hitPos).Normalize();
	Vector3 viewDir = (scene->cam.pos - hitPos).Normalize();
	Vector3 reflectDir = Vector3::Reflect(-lightDir, normal);

	Color diffuseColor = fmaxf(0.0f, lightDir.Dot(normal)) * light.color;
	diffuseColor *= mat.GetKd();
	Color specularColor = (specularExp + 1.0f) / (2.0f * PI) * powf(fmaxf(0.0f, reflectDir.Dot(viewDir)), specularExp) * mat.specularColor;
	specularColor *= mat.GetKs();

	return mat.diffuseColor * (ambientColor + diffuseColor) + specularColor;
}

bool Raytracer::IsInShadow(const Vector3& hitPos, const Light& light)
{
	Vector3 rayDir = (light.pos - hitPos).Normalize();
	Ray shadowRay(hitPos, rayDir);
	Intersection intersection = ComputeFirstRayObjectIntersection(shadowRay);

	// Commented out calculations differs in range 10e-7
	float lightDistance = (light.pos - hitPos).Dot(rayDir);	//(light.pos - hitPos).Magnitude();

	// Test if anything blocks the light
	return intersection.idx != 0 && intersection.distance < lightDistance;
}

// TODO: Fix transmission / refraction: Can be done by saving last material in ray data
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

	Material mat = scene->objects[intersection.idx]->material;
	Vector3 hitPos = ray.direction * intersection.distance + ray.origin;
	Vector3 normal = scene->objects[intersection.idx]->getNormalAt(hitPos);

	color = EvaluateLocalLightingModel(hitPos, normal, mat);

	// Early bail out for purely diffuse surfaces
	if (mat.GetKs() < EPS && mat.GetKt() < EPS)
		return color;

	// Reflection
	Color reflectionColor;
	if (mat.GetKs() > EPS)
	{
		Vector3 reflectDir = Vector3::Reflect(ray.direction, normal);
		Ray reflectRay = Ray(hitPos, reflectDir, ray.bounce + 1);
		if (mat.glossiness > 0.0f)
		{
			Vector3 disturbtion = 0.5f * Vector3((float)rand() / RAND_MAX,
				(float)rand() / RAND_MAX,
				(float)rand() / RAND_MAX) - 0.5f * Vector3(1, 1, 1);
			reflectRay.direction += mat.glossiness * disturbtion;
		}
		reflectionColor = mat.GetKs() * Traverse(reflectRay);
	}

	// Transmission / Refraction
	Color transmissionColor;
	if (mat.GetKt() > EPS)
	{
		bool leavingObject = normal.Dot(ray.direction) > 0;
		float n1 = leavingObject ? mat.refractiveIndex : 1.0f;
		float n2 = leavingObject ? 1.0f : mat.refractiveIndex;
		// Total Internal Refelction (TIR)
		float R = Vector3::FresnelReflectance(ray.direction, normal, n1, n2);	// Reflection percentage
		float T = 1.0f - R;	// Transmission percentage
		if (T > EPS)
		{
			// TBD: Do we need the EPS vector offset in transmission?
			Vector3 transmissionDir = Vector3::Refract(ray.direction, normal, n1, n2);
			Ray transmissionRay = Ray(hitPos + transmissionDir * EPS, transmissionDir, ray.bounce + 1);
			transmissionColor += T * mat.GetKt() * Traverse(transmissionRay);
		}
		if (R > EPS)
		{
			Vector3 reflectDir = Vector3::Reflect(ray.direction, normal);
			Ray reflectRay = Ray(hitPos, reflectDir, ray.bounce + 1);
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

	omp_set_num_threads(8);	// Number of logical CPU kernels

	int progress = 0;
	// Render scene to image
	#pragma omp parallel
	for (int y = 0; y < img.height; y++)
	{
		#pragma omp for nowait
		for (int x = 0; x < img.width; x++)
		{
			int idx = y * img.width + x;
			Color color;
			Ray camRay(cam.pos, cam.PixelToRayDir(x, y));
			color += Traverse(camRay);
			float xOffset, yOffset;	// Must be in range(-0.5,0.5)
			for (int s = 1; s < samplesPerPixel; s++)
			{
				// Simple random sampling, better sampling methods achieve better results 
				xOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				yOffset = (float)rand() / (float)RAND_MAX - 0.5f;
				camRay = Ray(cam.pos, cam.PixelToRayDir(x, y, xOffset, yOffset));
				color += Traverse(camRay);
			}
			// Normalize color and set pixel in image
			color /= static_cast<float>(samplesPerPixel);
			img.SetPixel(idx, color);
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