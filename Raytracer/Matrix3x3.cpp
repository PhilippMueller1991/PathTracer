#include "Matrix3x3.h"
#include <assert.h>
#include <math.h>

Vector3 Matrix3x3::GetRow(int idx) const
{
	assert(idx < 3);
	const int offset = idx * 3;
	return Vector3(m[offset], m[offset + 1], m[offset + 2]);
}

Vector3 Matrix3x3::GetColumn(int idx) const
{
	assert(idx < 3);
	return Vector3(m[0 + idx], m[3 +idx], m[6 + idx]);
}

Matrix3x3 Matrix3x3::Eye()
{
	Matrix3x3 mat;
	mat[0] = mat[4] = mat[8] = 1.0f;
	return mat;
}

Matrix3x3 Matrix3x3::Transpose() const
{
	Matrix3x3 mat;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mat[i * 3 + j] = this->m[j * 3 + i];
	return mat;
}