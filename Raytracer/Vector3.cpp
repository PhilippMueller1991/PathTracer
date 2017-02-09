#include "Vector3.h"
#include <math.h>

// Left handed coordinate system
const Vector3 Vector3::right = Vector3(1, 0, 0);
const Vector3 Vector3::up = Vector3(0, 1, 0);
const Vector3 Vector3::forward = Vector3(0, 0, 1);

float Vector3::Magnitude() const
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector3::Dot(const Vector3& b) const
{
	return x * b.x + y * b.y + z * b.z;
}

Vector3 Vector3::Cross(const Vector3& b) const
{
	return Vector3(y * b.z - b.y * z,
		z * b.x - b.z * x,
		x * b.y - b.x * y);
}

Vector3 Vector3::Normalize() const
{
	float length = Magnitude();
	return *this / length;
}

Vector3 Vector3::Reflect(Vector3 v, Vector3 n)
{
	return (2*(v.Dot(n))*n-v);
}
