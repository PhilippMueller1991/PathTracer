#include "Vector3.h"
#include <math.h>

// Left handed coordinate system
// Define static const values
const Vector3 Vector3::invalid = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
const Vector3 Vector3::right = Vector3(1, 0, 0);
const Vector3 Vector3::up = Vector3(0, 1, 0);
const Vector3 Vector3::forward = Vector3(0, 0, 1);

float Vector3::Magnitude() const
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector3::MagnitudeSqr() const
{
	return x*x + y*y + z*z;
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

Vector3 Vector3::Reflect(const Vector3& incident, const Vector3& normal)
{
	return incident - 2.0f * (incident.Dot(normal))*normal;
}

// Calculates the exitence vector based on incident and normal vector 
// as well as materials n1, n2 index of refraction
Vector3 Vector3::Refract(const Vector3& incident, const Vector3& normal, float n1, float n2)
{
	float n = n1 / n2;
	float cosI = -incident.Dot(normal);
	float sinT2 = n * n * (1.0f - cosI * cosI);

	// Total internal refraction
	if (sinT2 > 1.0f)
		return Vector3::invalid;

	float cosT = sqrtf(1.0f - sinT2);
	
	return n * incident + (n * cosI - cosT) * normal;
}

// Calculates the relation of reflectance vs refraction based on
// incident and normal vector as well as materials n1, n2 index of refraction
float Vector3::FresnelReflectance(const Vector3& incident, const Vector3& normal, float n1, float n2)
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
