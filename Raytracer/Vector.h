// Left handed coordinate system
#pragma once

#include <iostream>

#define EPS 0.0001f

template <typename T, int dim>
class Vector
{
};

template<typename T>
class Vector<T, 3>
{
public:
	T x, y, z;

public:
	Vector(T a = 0, T b = 0, T c = 0)
		: x(a), y(b), z(c)
	{
	}

	// Operator overload
	inline bool operator==(const Vector& rhs)
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	inline bool operator!=(const Vector& rhs)
	{
		return !(*this == rhs);
	}
	inline Vector operator+(const Vector& rhs) const
	{
		return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	inline Vector operator-(const Vector& rhs) const
	{
		return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	inline void operator+=(const Vector& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	inline void operator-=(const Vector& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	inline Vector operator*(T rhs) const
	{
		return Vector(x * rhs, y * rhs, z * rhs);
	}
	inline friend Vector operator*(T lhs, const Vector& rhs)
	{
		return Vector(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}
	inline void operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
	}
	inline Vector operator/(T rhs) const
	{
		return Vector(x / rhs, y / rhs, z / rhs);
	}
	inline friend Vector operator/(T lhs, const Vector& rhs)
	{
		return Vector(rhs.x / lhs, rhs.y / lhs, rhs.z / lhs);
	}
	inline void operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
	}
	inline Vector operator-()
	{
		return Vector(-x, -y, -z);
	}
	inline friend std::ostream& operator<<(std::ostream& os, const Vector& obj)
	{
		os << "Vec3(" << obj.x << "," << obj.y << "," << obj.z << ")";
		return os;
	}

	// Auxiliary methods
	float Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
	float MagnitudeSqr() const
	{
		return x * x + y * y + z * z;
	}
	float Dot(const Vector& b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}
	Vector Cross(const Vector& b) const
	{
		return Vector(y * b.z - b.y * z,
			z * b.x - b.z * x,
			x * b.y - b.x * y);
	}
	Vector Normalize() const
	{
		return *this / Magnitude();
	}

	static Vector Reflect(const Vector& incident, const Vector& normal)
	{
		return incident - 2.0f * (incident.Dot(normal))*normal;
	}
	static Vector Refract(const Vector& incident, const Vector& normal, float n1, float n2)
	{
		float n = n1 / n2;
		float cosI = -incident.Dot(normal);
		float sinT2 = n * n * (1.0f - cosI * cosI);

		// Total internal refraction
		if (sinT2 > 1.0f)
			return Vector::invalid;

		float cosT = sqrtf(1.0f - sinT2);

		return n * incident + (n * cosI - cosT) * normal;
	}
	static float FresnelReflectance(const Vector& incident, const Vector& normal, float n1, float n2)
	{
		float n = n1 / n2;
		float cosI = -incident.Dot(normal);
		float sinT2 = n * n * (1.0f - cosI * cosI);

		// Total internal refraction
		if (sinT2 > 1.0f)
			return 1.0f;

		float cosT = sqrtf(1.0f - sinT2);
		float rOrthogonal = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
		float rParallel = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);

		return (rOrthogonal * rOrthogonal + rParallel * rParallel) / 2.0f;
	}

	// Predefined vectors
	const static Vector invalid;
	const static Vector right;
	const static Vector up;
	const static Vector forward;
	const static Vector zero;
	const static Vector one;
};

template<typename T>
class Vector<T, 2>
{
public:
	T x, y;

public:
	Vector(T a = 0, T b = 0)
		: x(a), y(b)
	{
	}

	// Operator overload
	inline bool operator==(const Vector& rhs)
	{
		return x == rhs.x && y == rhs.y;
	}
	inline bool operator!=(const Vector& rhs)
	{
		return !(*this == rhs);
	}
	inline Vector operator+(const Vector& rhs) const
	{
		return Vector(x + rhs.x, y + rhs.y);
	}
	inline Vector operator-(const Vector& rhs) const
	{
		return Vector(x - rhs.x, y - rhs.y);
	}
	inline void operator+=(const Vector& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	inline void operator-=(const Vector& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	inline Vector operator*(T rhs) const
	{
		return Vector(x * rhs, y * rhs);
	}
	inline friend Vector operator*(T lhs, const Vector& rhs)
	{
		return Vector(lhs * rhs.x, lhs * rhs.y);
	}
	inline void operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
	}
	inline Vector operator/(T rhs) const
	{
		return Vector(x / rhs, y / rhs);
	}
	inline friend Vector operator/(T lhs, const Vector& rhs)
	{
		return Vector(rhs.x / lhs, rhs.y / lhs);
	}
	inline void operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
	}
	inline Vector operator-()
	{
		return Vector(-x, -y);
	}
	inline friend std::ostream& operator<<(std::ostream& os, const Vector& obj)
	{
		os << "Vec2(" << obj.x << "," << obj.y << ")";
		return os;
	}

	// Auxiliary methods
	float Magnitude() const
	{
		return sqrtf(x * x + y * y);
	}
	float MagnitudeSqr() const
	{
		return x * x + y * y;
	}
	float Dot(const Vector& b) const
	{
		return x * b.x + y * b.y;
	}
	float Area(const Vector& b) const
	{
		return x * b.y - y * b.x;
	}
	Vector Cross() const
	{
		return Vector(y, -x);
	}
	Vector Normalize() const
	{
		return *this / Magnitude();
	}

	// Predefined vectors
	const static Vector invalid;
	const static Vector right;
	const static Vector up;
	const static Vector zero;
	const static Vector one;
};

typedef Vector<float, 3> Vec3f;
typedef Vector<float, 2> Vec2f;