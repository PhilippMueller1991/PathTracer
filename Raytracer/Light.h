#pragma once

#include "Vector3.h"
#include "Color.h"

// For now only point light
class Light
{
public:
	Vector3 pos;
	Color color;
	float intensity;

	Light(Vector3 position, Color color = Color(1, 1, 1), float intensity = 0.5f)
		: pos(position), color(color), intensity(intensity)
	{
		//this->color = intensity * color;
	}
	~Light() {}
};

