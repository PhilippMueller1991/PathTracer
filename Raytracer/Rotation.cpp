#include "Rotation.h"

Rotation Rotation::RotateX(float a)
{
	Rotation m;
	m[0] = 1;		m[1] = 0;			m[2] = 0;
	m[3] = 0;		m[4] = cosf(a);		m[5] = -sinf(a);
	m[6] = 0;		m[7] = sinf(a);		m[8] = cosf(a);
	return m;
}
Rotation Rotation::RotateY(float a)
{
	Rotation m;
	m[0] = cosf(a);		m[1] = 0;		m[2] = sinf(a);
	m[3] = 0;			m[4] = 1;		m[5] = 0;
	m[6] = -sinf(a);	m[7] = 0;		m[8] = cosf(a);
	return m;
}
Rotation Rotation::RotateZ(float a)
{
	Rotation m;
	m[0] = cosf(a);		m[1] = -sinf(a);	m[2] = 0;
	m[3] = sinf(a);		m[4] = cosf(a);		m[5] = 0;
	m[6] = 0;			m[7] = 0;			m[8] = 1;
	return m;
}

Rotation Rotation::EulerAngles(float a, float b, float c)
{
	return RotateX(a) * RotateY(b) * RotateZ(c);
}

// v = vector to rotate
// r = rotation axis, normalized
// See http://ksuweb.kennesaw.edu/~plaval//math4490/rotgen.pdf
Vector3 Rotation::RotateAround(Vector3 v, Vector3 r, float angle)
{
	r = r.Normalize();
	const float C = cosf(angle);
	const float S = sinf(angle);
	const float t = 1 - C;

	Rotation rot;
	rot[0] = t * r.x * r.x + C;			rot[1] = t * r.x * r.y - S *r.z;	rot[2] = t * r.x *r.z + S * r.y;
	rot[3] = t * r.x * r.y + S * r.z;	rot[4] = t * r.y * r.y + C;			rot[5] = t * r.y * r.z - S * r.x;
	rot[6] = t * r.x * r.z - S * r.y;	rot[7] = t * r.y * r.z + S *r.x;	rot[8] = t * r.z * r.z + C;	
	return rot * v;
}

void MirrorHand(Rotation& a)
{
	//a[6] = -a[6];
	//a[7] = -a[7];
	//a[2] = -a[2];
	//a[3] = -a[3];
	Rotation p;
	p[0] = 1.0f;
	p[5] = 1.0f;
	p[7] = 1.0f;

	a = p.Inverse() * a * p;
}

// OPTIONAL TODO: Can be further optimized
Rotation Rotation::RotationBetween(Vector3 from, Vector3 to)
{
	from = from.Normalize();
	to = to.Normalize();

	const Vector3 v = to.Cross(from);	// Rotation axis
	const float ca = to.Dot(from);
	const Vector3 vs = to.Cross(from);
	const float t = 1.0f - ca;
	const Vector3 vt = t * v;

	Rotation rot;
	// Diagonal
	rot[0] = vt.x * v.x + ca;
	rot[4] = vt.y * v.y + ca;
	rot[8] = vt.z * v.z + ca;
	// Fill remaining fields
	rot[1] = vt.x * v.y - vs.z;
	rot[2] = vt.x * v.z + vs.y;
	rot[3] = vt.x * v.y + vs.z;
	rot[5] = vt.y * v.z - vs.x;
	rot[6] = vt.x * v.z - vs.y;
	rot[7] = vt.y * v.z + vs.x;

	// DEBUG
	// Swap y and z axis for handedness
	//MirrorHand(rot);

	return rot;
}

Rotation Rotation::Inverse() const
{
	Rotation rot;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rot[i * 3 + j] = this->m[j * 3 + i];
	return rot;
}