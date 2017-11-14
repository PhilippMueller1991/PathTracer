#pragma once

#include "Material.h"

// TODO: Use start and end materials for correct refraction
class Intersection
{
public:
	int idx;
	float distance;
	Material* start;
	Material* end;
public:
	Intersection(int idx, float distance) 
		: idx(idx), distance(distance)
	{
	}
	~Intersection()
	{
	}
};

