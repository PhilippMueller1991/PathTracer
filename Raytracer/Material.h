#pragma once

#include "Ray.h"
#include "Vector.h"
#include "Color.h"
#include "Texture.h"

#include <assert.h>
#include <memory>

class Material
{
public:
	Color diffuseColor;
	Color specularColor;
	float refractiveIndex;
	float glossiness;
	std::shared_ptr<Texture> texture;

private:
	float kd, ks, kt;

public:
	// Rather use mapping than enum
	//static const enum RefractionIndexType { REFRACTION_INDEX_GLASS, REFRACTION_INDEX_WATER, REFRACTION_INDEX_SIZE};

	Material(float diffuse = 1.0f, float specular = 0.0f, float transmisson = 0.0f,
		Color diffuseColor = Color(1, 1, 1), Color specularColor = Color(1, 1, 1),
		float refractiveIndex = 1.52f, float glossiness = 0.0f, std::shared_ptr<Texture> texture = nullptr)
		: refractiveIndex(refractiveIndex), texture(texture)
	{
		float sum = specular + diffuse + transmisson;
		assert(sum != 0);

		kd = diffuse / sum;
		ks = specular / sum;
		kt = transmisson / sum;

		glossiness = glossiness < 0.0f ? -glossiness : glossiness;
		this->glossiness = glossiness > 1.0f ? 1.0f : glossiness;
		this->diffuseColor = kd * diffuseColor;
		this->specularColor = ks * specularColor;
	}
	~Material() {}

	inline Ray::RayType chooseRandomRayType();
	Vec3f DisturbeReflectionDir(Vec3f reflectionDir) const;

	// Getter and setter
	float getKd() const { return kd; }
	float getKs() const { return ks; }
	float getKt() const { return kt; }
	Color getColorAt(float u, float v) const;
	void setTexture(std::shared_ptr<Texture> tex);
};

