#pragma once

#include "Vector3.h"
#include "Color.h"

// For now only directional light
class Light
{
public:
	Vector3 pos;
	Color color;

	Light() : pos(Vector3(0,0,0)), color(Color(1,1,1,0)) {}
	Light(Vector3 position, Color color) : pos(position), color(color) {}
	~Light() {}
};

