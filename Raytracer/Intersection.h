#pragma once

#include "Material.h"

class Intersection
{
public:
	int idx;
	float distance;
public:
	Intersection(int idx, float distance) 
		: idx(idx), distance(distance)
	{
	}
	~Intersection()
	{
	}
};

