#pragma once

#include "Vector3.h"
#include "Rotation.h"

class Transform
{
public:
	Vector3 pos;
	Rotation rot;
	Vector3 scale;

	Transform(Vector3 pos = Vector3(0, 0, 0), Rotation rot = Rotation::Eye(), Vector3 scale = Vector3(1, 1, 1))
		: pos(pos), rot(rot), scale(scale)
	{
	}
	~Transform()
	{
	}
};

