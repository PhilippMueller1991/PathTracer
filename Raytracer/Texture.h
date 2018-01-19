#pragma once

#include "Vector3.h"
#include "Color.h"

class Texture
{
public:
	float scale;
public:
	Texture(float scale = 1.0f)
		: scale(scale)
	{
	}
	~Texture()
	{
	}

	virtual Color GetColorAt(float u, float v); // UV values are in range(0,1)
};

