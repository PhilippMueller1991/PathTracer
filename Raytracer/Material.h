#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Color.h"
#include "Texture.h"

#include <assert.h>

class Material
{
public:
	Color diffuseColor;
	Color specularColor;
	float refractiveIndex;
	Texture* texture;

private:
	float kd, ks, kt;

public:
	// Rather use mapping than enum
	//static const enum RefractionIndexType { REFRACTION_INDEX_GLASS, REFRACTION_INDEX_WATER, REFRACTION_INDEX_SIZE};

	Material(float diffuse = 1.0f, float specular = 0.0f, float transmisson = 0.0f, 
		Color diffuseColor = Color(1, 1, 1), Color specularColor = Color(1, 1, 1), 
		float refractiveIndex = 1.52f, Texture* texture=nullptr)
		: refractiveIndex(refractiveIndex), texture(texture)
	{
		float sum = specular + diffuse + transmisson;
		assert(sum != 0);

		kd = diffuse / sum;
		ks = specular / sum;
		kt = transmisson / sum;

		this->diffuseColor = kd * diffuseColor;
		this->specularColor = ks * specularColor;
	}
	~Material() {}

	inline Ray::RayType chooseRandomRayType();

	// Getter and setter
	float GetKd() const { return kd; }
	float GetKs() const { return ks; }
	float GetKt() const { return kt; }
	Color GetColorAt(float u, float v) const;	// UV values are in range(0,1)
	Color GetColorAt(Vector3 pos) const;
};

