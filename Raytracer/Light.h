#pragma once

#include "Vector3.h"
#include "Color.h"

// For now only point light
class Light
{
public:
	Vector3 pos;
	Color color;

	Light() 
		: pos(Vector3(0,0,0)), color(Color(1,1,1))
	{
	}
	Light(Vector3 position, Color color = Color(1, 1, 1), float intensity = 0.5f) 
		: pos(position)
	{
		this->color = intensity * color;
	}
	~Light() {}
};

