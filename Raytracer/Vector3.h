#pragma once

#include <iostream>

#define EPS 0.001f


// Left handed coordinate system
class Vector3
{
public:
	float x, y, z;

	Vector3()
		: x(0), y(0), z(0)
	{
	}
	Vector3(float a, float b, float c)
		: x(a), y(b), z(c)
	{
	}
	~Vector3() {}

	// Operator overload
	bool operator==(const Vector3& rhs)
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const Vector3& rhs)
	{
		return !(*this == rhs);
	}
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
	float MagnitudeSqr() const;
	float Dot(const Vector3& b) const;
	Vector3 Cross(const Vector3& b) const;
	Vector3 Normalize() const;
	
	static Vector3 Reflect(const Vector3& incident, const Vector3& normal);
	static Vector3 Refract(const Vector3& incident, const Vector3& normal, float n1, float n2);
	static float FresnelReflectance(const Vector3& incident, const Vector3& normal, float n1, float n2);

	// Predefined vectors
	static const Vector3 invalid;
	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;
};
