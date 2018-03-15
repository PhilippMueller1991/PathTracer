#include "Rotation.h"

Rotation Rotation::rotateX(float a)
{
	Rotation m;
	m[0] = 1;		m[1] = 0;			m[2] = 0;
	m[3] = 0;		m[4] = cosf(a);		m[5] = -sinf(a);
	m[6] = 0;		m[7] = sinf(a);		m[8] = cosf(a);
	return m;
}
Rotation Rotation::rotateY(float a)
{
	Rotation m;
	m[0] = cosf(a);		m[1] = 0;		m[2] = sinf(a);
	m[3] = 0;			m[4] = 1;		m[5] = 0;
	m[6] = -sinf(a);	m[7] = 0;		m[8] = cosf(a);
	return m;
}
Rotation Rotation::rotateZ(float a)
{
	Rotation m;
	m[0] = cosf(a);		m[1] = -sinf(a);	m[2] = 0;
	m[3] = sinf(a);		m[4] = cosf(a);		m[5] = 0;
	m[6] = 0;			m[7] = 0;			m[8] = 1;
	return m;
}

Rotation Rotation::eulerAngles(float a, float b, float c)
{
	return rotateX(a) * rotateY(b) * rotateZ(c);
}

// v = vector to rotate
// r = rotation axis, normalized
// See http://ksuweb.kennesaw.edu/~plaval//math4490/rotgen.pdf
Vec3f Rotation::rotateAround(Vec3f v, Vec3f r, float angle)
{
	r = r.normalize();
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

	a = p.inverse() * a * p;
}

// TODO: Can be further optimized
Rotation Rotation::rotationBetween(Vec3f from, Vec3f to)
{
	from = from.normalize();
	to = to.normalize();

	const Vec3f v = to.cross(from);	// Rotation axis
	const float ca = to.dot(from);
	const Vec3f vs = to.cross(from);
	const float t = 1.0f - ca;
	const Vec3f vt = t * v;

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

	return rot;
}

Rotation Rotation::inverse() const
{
	Rotation rot;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rot[i * 3 + j] = this->m[j * 3 + i];
	return rot;
}