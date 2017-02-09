#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Color.h"

#include <assert.h>

class Material
{
public:
	float ks, kd, kt;
	float specularExp;
	Color diffuseColor;
	Color specularColor;

	Material(float diffuse = 1.0f, float specular = 0.0f, float transmisson = 0.0f, Color diffuseColor = Color(1, 1, 1), Color specularColor = Color(1, 1, 1), float specularExp = 50.0f)
		: diffuseColor(diffuseColor), specularColor(specularColor), specularExp(specularExp)
	{
		float sum = specular + diffuse + transmisson;

		assert(sum != 0);

		// Normalize
		ks = specular / sum;
		kd = diffuse / sum;
		kt = transmisson / sum;
	}
	~Material() {}

	inline Ray::RayType chooseRandomRayType();
};

