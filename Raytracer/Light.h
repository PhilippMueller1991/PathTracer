#pragma once

#include "Vector.h"
#include "Color.h"

// For now only point light
class Light
{
public:
	Vec3f pos;
	Color color;
	float intensity;

	Light(Vec3f position, Color color = Color(1, 1, 1), float intensity = 0.5f)
		: pos(position), color(color), intensity(intensity)
	{
		//this->color = intensity * color;
	}
	~Light() {}
};

