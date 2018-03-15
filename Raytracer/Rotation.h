#pragma once

#include "Vector.h"
#include "Matrix3x3.h"

class Rotation : public Matrix3x3
{
public:
	Rotation()
	{
	}
	~Rotation()
	{
	}
	Rotation(Matrix3x3& mat)
	{
		for (int i = 0; i < 9; i++)
			this->m[i] = mat[i];
	}

	static Rotation rotateX(float a);
	static Rotation rotateY(float a);
	static Rotation rotateZ(float a);
	static Rotation eulerAngles(float a, float b, float c);
	static Rotation rotationBetween(Vec3f from, Vec3f to);
	static Vec3f rotateAround(Vec3f v, Vec3f r, float angle);

	Rotation inverse() const;
};

