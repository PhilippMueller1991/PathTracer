#pragma once

#include "Vector3.h"
#include "Color.h"

// For now only directional light
class Light
{
public:
	Vector3 pos;
	Color color;
	float intensity;

	Light() : pos(Vector3(0,0,0)), color(Color(1,1,1)), intensity(0.5f) {}
	Light(Vector3 position, Color color = Color(1, 1, 1), float intensity = 0.5f) 
		: pos(position), color(color), intensity(intensity) {}
	~Light() {}
};

