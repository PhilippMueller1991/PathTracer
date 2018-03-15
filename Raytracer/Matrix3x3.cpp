#include "Matrix3x3.h"
#include <assert.h>
#include <math.h>

Vec3f Matrix3x3::getRow(int idx) const
{
	assert(idx < 3);
	const int offset = idx * 3;
	return Vec3f(m[offset], m[offset + 1], m[offset + 2]);
}

Vec3f Matrix3x3::getColumn(int idx) const
{
	assert(idx < 3);
	return Vec3f(m[0 + idx], m[3 +idx], m[6 + idx]);
}

Matrix3x3 Matrix3x3::eye()
{
	Matrix3x3 mat;
	mat[0] = mat[4] = mat[8] = 1.0f;
	return mat;
}

Matrix3x3 Matrix3x3::transpose() const
{
	Matrix3x3 mat;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mat[i * 3 + j] = this->m[j * 3 + i];
	return mat;
}