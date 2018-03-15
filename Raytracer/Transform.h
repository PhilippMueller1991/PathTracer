#pragma once

#include "Vector.h"
#include "Rotation.h"

class Transform
{
public:
	Vec3f pos;
	Rotation rot;
	Vec3f scale;

	Transform(Vec3f pos = Vec3f(0, 0, 0), Rotation rot = Rotation::eye(), Vec3f scale = Vec3f(1, 1, 1))
		: pos(pos), rot(rot), scale(scale)
	{
	}
	~Transform()
	{
	}

	// Operator decleration
	// TODO: Test
	Vec3f operator*(Vec3f rhs)
	{
		Vec3f v = rhs - pos;
		v = rot * v;
		v.x *= scale.x;
		v.y *= scale.y;
		v.z *= scale.z;
		return v;
	}
};

