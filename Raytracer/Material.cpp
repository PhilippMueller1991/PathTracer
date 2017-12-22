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

Vector3 Material::DisturbeReflectionDir(Vector3 reflectionDir) const
{
	if (glossiness <= EPS)
		return reflectionDir;

	Vector3 disturbtion = 0.5f * Vector3((float)rand() / RAND_MAX,
		(float)rand() / RAND_MAX,
		(float)rand() / RAND_MAX) - 0.5f * Vector3(1, 1, 1);

	return (reflectionDir + glossiness * disturbtion).Normalize();
}

// Getter and setter
// UV values are in range(0,1)
Color Material::GetColorAt(float u, float v) const
{
	if (texture == nullptr)
		return diffuseColor;

	return texture->GetColorAt(u, v);
}

Color Material::GetColorAt(Vector3 pos) const
{
	if (texture == nullptr)
		return diffuseColor;

	return texture->GetColorAt(pos);
}

void SetTexture(Texture* tex)
{

}