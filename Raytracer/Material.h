#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Color.h"

#include <assert.h>

class Material
{
private:
	float ks;	// specular ray spawn probability
	float kd;	// diffuse ray spawn probability
	float kt;	// transmission ray spawn probablity

public:
	Color color;

	Material(float specular, float diffuse, float transmisson, Color color = Color(0,0,0,0)) : color(color)
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

