#pragma once

#include <vector>

#include "SceneObject.h"
#include "MeshLoader.h"

struct Triangle {
public:
	static inline Vec3f GetNormal(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
	{
		return (v2 - v0).Cross(v1 - v0).Normalize();
	}
	
	// Modified Moeller-Trumbore intersection algorithm
	// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	static bool FindIntersection(Ray& ray, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, float& dist)
	{
		Vec3f edge1 = v1 - v0;
		Vec3f edge2 = v2 - v0;
		// Is triangle parallel to rd
		Vec3f h = ray.direction.Cross(edge2);
		float a = edge1.Dot(h);
		if (a > -EPS && a < EPS)
			return false;
		Vec3f s = ray.origin - v0;
		float f = 1.0f / a;
		float u = f * s.Dot(h);
		if (u < 0.0f || u > 1.0f)
			return false;
		Vec3f q = s.Cross(edge1);
		float v = f * ray.direction.Dot(q);
		if (v < 0.0f || u + v > 1.0f)
			return false;
		float t = f * edge2.Dot(q);
		if (t > EPS) // Ray intersection
		{
			dist = t;
			return true;
		}
		else // This means that there is a line intersection but not a ray intersection.
			return false;
	}

	static bool IsOnSurface(const Vec3f& pos, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
	{
		// Barycentric coordinates with early exit
		float det = (v1.y - v2.y)*(v0.x - v2.x) + (v2.x - v1.x)*(v0.y - v2.y);
		// a0
		float a0 = (v1.y - v2.y)*(pos.x - v2.x) + (v2.x - v1.x)*(pos.y - v2.y);
		a0 /= det;
		if (a0 < 0.0f || a0 > 1.0f)
			return false;
		// a1
		float a1 = (v2.y - v0.y)*(pos.x - v2.x) + (v0.x - v2.x)*(pos.y - v2.y);
		a1 /= det;
		if (a1 < 0.0f || a1 > 1.0f)
			return false;
		// a2
		float a2 = 1.0f - a0 - a1;
		if (a2 < 0.0f || a2 > 1.0f)
			return false;

		return true;
	}

private:
	// Barycentric coordinates
	// https://en.wikipedia.org/wiki/Barycentric_coordinate_system
	static Vec3f GetBarycentricCoordinates(const Vec3f& pos, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
	{
		// Calculate determinant
		float det = (v1.y - v2.y)*(v0.x - v2.x) + (v2.x - v1.x)*(v0.y - v2.y);
		// a0
		float a0 = (v1.y - v2.y)*(pos.x - v2.x) + (v2.x - v1.x)*(pos.y - v2.y);
		a0 /= det;
		// a1
		float a1 = (v2.y - v0.y)*(pos.x - v2.x) + (v0.x - v2.x)*(pos.y - v2.y);
		a1 /= det;
		// a2
		float a2 = 1.0f - a0 - a1;

		return Vec3f(a0, a1, a2);
	}
};

class Mesh : public SceneObject
{
private:
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> uvs;
	std::vector<uint32_t> vidxs;
	std::vector<uint32_t> uvidxs;
	std::vector<uint32_t> nidxs;

public:
	Mesh(Vec3f pos = Vec3f::zero, Rotation rot = Rotation::Eye(), Vec3f scale = Vec3f::one, Material m = Material(1, 2))
		: SceneObject(Transform(pos, rot, scale), m)
	{
	}

	// Virtual function overrides
	virtual Vec3f GetNormalAt(Vec3f pos) override;
	virtual Color GetColorAt(Vec3f pos) override;
	virtual bool FindIntersection(Ray& ray, float& dist) override;

	bool LoadOBJ(const std::string& fileName);
	void PrintBuffers();

private:
	void ClearBuffers();
};