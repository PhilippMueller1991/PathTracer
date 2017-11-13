#pragma once

#include "Vector3.h"

class Matrix3x3
{
protected:
	float m[9];

public:
	Matrix3x3()
	{
		for (int i = 0; i < 9; i++)
			m[i] = 0.0f;
	}
	~Matrix3x3()
	{
	}

	// Operators
	float& operator[](std::size_t idx)
	{
		return m[idx];
	}
	Matrix3x3 operator*(const Matrix3x3& rhs) const
	{
		Matrix3x3 mat;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int ij = i * 3 + j;
				for (int k = 0; k < 3; k++)
				{
					int ik = i * 3 + k;
					int kj = k * 3 + j;
					mat[ij] += this->m[ik] * rhs.m[kj];
				}
			}
		}
		return mat;
	}
	Matrix3x3 operator+(const Matrix3x3& rhs) const
	{
		Matrix3x3 mat;
		for (int i = 0; i < 9; i++)
		{
			mat.m[i] = this->m[i] + rhs.m[i];
		}
		return mat;
	}
	Matrix3x3 operator-(const Matrix3x3& rhs) const
	{
		Matrix3x3 mat;
		for (int i = 0; i < 9; i++)
		{
			mat.m[i] = this->m[i] - rhs.m[i];
		}
		return mat;
	}
	Matrix3x3 operator+=(const Matrix3x3& rhs)
	{
		for (int i = 0; i < 9; i++)
		{
			this->m[i] += rhs.m[i];
		}
		return *this;
	}
	Matrix3x3 operator-=(const Matrix3x3& rhs)
	{
		for (int i = 0; i < 9; i++)
		{
			this->m[i] -= rhs.m[i];
		}
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& os, const Matrix3x3& rhs)
	{
		for (int i = 0; i < 3; i++)
			os << rhs.m[i * 3] << ", " << rhs.m[i * 3 + 1] << ", " << rhs.m[i * 3 + 2] << "\n";
		return os;
	}
	Vector3 operator*(const Vector3& rhs) const
	{
		float r[] = { rhs.x, rhs.y, rhs.z };
		float res[] = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; i++)
		{
			for (int k = 0; k < 3; k++)
			{
				res[i] += this->m[i * 3 + k] * r[k];
			}
		}
		return Vector3(res[0], res[1], res[2]);
	}

	// Get row and column no tested!
	Vector3 GetRow(int idx) const;
	Vector3 GetColumn(int idx) const;
	Matrix3x3 Transpose() const;
	static Matrix3x3 Eye();
};

