#pragma once

#include "Material.h"

class Intersection
{
public:
	int idx;
	float distance;
public:
	Intersection(int idx = -1, float distance = FLT_MAX)
		: idx(idx), distance(distance)
	{
	}
	~Intersection()
	{
	}
};

