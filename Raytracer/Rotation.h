#pragma once

#include "Vector3.h"
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

	static Rotation RotateX(float a);
	static Rotation RotateY(float a);
	static Rotation RotateZ(float a);
	static Rotation EulerAngles(float a, float b, float c);
	static Rotation RotationBetween(Vector3 from, Vector3 to);
	static Vector3 RotateAround(Vector3 v, Vector3 r, float angle);

	Rotation Inverse() const;
};

