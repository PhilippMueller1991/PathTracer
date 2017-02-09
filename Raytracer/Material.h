#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Color.h"

#include <assert.h>

class Material
{
private:
	float kd, ks, kt;

public:
	float specularExp;
	Color diffuseColor;
	Color specularColor;

	Material(float diffuse = 1.0f, float specular = 0.0f, float transmisson = 0.0f, Color diffuseColor = Color(1, 1, 1), Color specularColor = Color(1, 1, 1), float specularExp = 50.0f)
		: specularExp(specularExp)
	{
		float sum = specular + diffuse + transmisson;
		assert(sum != 0);

		ks = specular / sum;
		kd = diffuse / sum;
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
};

