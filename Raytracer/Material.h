#pragma once

#include "Ray.h"
#include "Vector3.h"
#include "Color.h"

#include <assert.h>

class Material
{
public:
	float ks, kd, kt;
	Color color;

	Material(float specular = 1.0f, float diffuse = 1.0f, float transmisson = 0.0f, Color color = Color(0, 0, 0, 0)) : color(color)
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

