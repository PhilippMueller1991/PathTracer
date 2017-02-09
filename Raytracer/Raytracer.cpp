#include "Raytracer.h"


int Raytracer::maxBounces = 1;

const float ambientIntensity = 0.1f;
const Color ambientColor(0.9f, 0.9f, 1);

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

Color Raytracer::EvaluateLocalLightingModel(const Vector3& normal, const Material& mat, const Light& light)
{
	// FOR DEBUG RETURN MATERIAL COLOR
	return mat.diffuseColor;

	//Color color = ambientIntensity * ambientColor;
	//return color;
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
	Color color(1, 1, 1);
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
	// Early exit for normals that point away from current light
	bool inShadow = normal.Dot(light.pos - hitPos) < 0;
	if (!inShadow && !IsInShadow(hitPos, light))
		color = EvaluateLocalLightingModel(normal, scene->objects[intersection.idx]->material, light);
	else
		color = 0.1f * EvaluateLocalLightingModel(normal, scene->objects[intersection.idx]->material, light);

	// Pathtracer: Needs additional material property diffuse in recursive call
	//Material mat;
	//return color + mat.ks * Traverse(reflect) + mat.kt * Traverse(refract);
	return color;
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
			Color color = Traverse(camRay, scene->objects, scene->lights);
			img.SetPixel(idx, color);
		}
	}

	std::cout << "Writing scene.bmp" << std::endl;
	img.SaveBitmap("..//scene.bmp");
}