#pragma once

#include <iostream>

#define EPS 0.001f

class Vector3
{
public:
	float x, y, z;

	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;

	Vector3() {}
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
	~Vector3() {}

	// Operator overload
	Vector3 operator+(const Vector3& rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Vector3 operator-(const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	void operator+=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	void operator-=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	Vector3 operator*(float rhs) const
	{
		return Vector3(x * rhs, y * rhs, z * rhs);
	}
	friend Vector3 operator*(float lhs, const Vector3& rhs)
	{
		return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}
	void operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
	}
	Vector3 operator/(float rhs) const
	{
		return Vector3(x / rhs, y / rhs, z / rhs);
	}
	friend Vector3 operator/(float lhs, const Vector3& rhs)
	{
		return Vector3(rhs.x / lhs, rhs.y / lhs, rhs.z / lhs);
	}
	void operator/=(float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
	}
	Vector3 operator-()
	{
		return Vector3(-x, -y, -z);
	}
	friend std::ostream& operator<<(std::ostream& os, const Vector3& obj)
	{
		os << "Vector3(" << obj.x << "," << obj.y << "," << obj.z << ")";
		return os;
	}

	// Auxiliary methods
	float Magnitude() const;
	float Dot(const Vector3& b) const;
	Vector3 Cross(const Vector3& b) const;
	Vector3 Normalize() const;
	
	static Vector3 Reflect(Vector3 v, Vector3 n);
};
