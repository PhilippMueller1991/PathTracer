#include "Material.h"

#include <random>

Ray::RayType Material::chooseRandomRayType()
{
	float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	if (x < kd)
		return Ray::RayType::RAY_DIFFUSE;
	if (x < kd + ks)
		return Ray::RayType::RAY_SPECULAR;

	return Ray::RayType::RAY_TRANSMISSION;
}

Vec3f Material::DisturbeReflectionDir(Vec3f reflectionDir) const
{
	if (glossiness <= EPS)
		return reflectionDir;

	Vec3f disturbtion = 0.5f * Vec3f((float)rand() / RAND_MAX,
		(float)rand() / RAND_MAX,
		(float)rand() / RAND_MAX) - 0.5f * Vec3f(1, 1, 1);

	return (reflectionDir + glossiness * disturbtion).normalize();
}

// Getter and setter
Color Material::getColorAt(float u, float v) const
{
	if (texture == nullptr)
		return diffuseColor;

	return texture->getColorAt(u,v);
}

void Material::setTexture(std::shared_ptr<Texture> tex)
{
	texture = tex;
}