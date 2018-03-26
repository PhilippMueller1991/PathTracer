#pragma once

#include <vector>

#include "SceneObject.h"
#include "MeshLoader.h"

struct Triangle {
public:
	static inline Vec3f getNormal(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
	{
		return (v2 - v0).cross(v1 - v0).normalize();
	}
	
	// Modified Moeller-Trumbore intersection algorithm
	// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	static bool findIntersection(Ray& ray, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, float& dist)
	{
		Vec3f edge1 = v1 - v0;
		Vec3f edge2 = v2 - v0;
		// Is triangle parallel to rd
		Vec3f h = ray.direction.cross(edge2);
		float a = edge1.dot(h);
		if (a > -EPS && a < EPS)
			return false;
		Vec3f s = ray.origin - v0;
		float f = 1.0f / a;
		float u = f * s.dot(h);
		if (u < 0.0f || u > 1.0f)
			return false;
		Vec3f q = s.cross(edge1);
		float v = f * ray.direction.dot(q);
		if (v < 0.0f || u + v > 1.0f)
			return false;
		float t = f * edge2.dot(q);
		if (t > EPS) // Ray intersection
		{
			dist = t;
			return true;
		}
		else // This means that there is a line intersection but not a ray intersection.
			return false;
	}

	static bool isOnSurface(const Vec3f& pos, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
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
	static Vec3f getBarycentricCoordinates(const Vec3f& pos, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2)
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
	Mesh(Vec3f pos = Vec3f::zero, Rotation rot = Rotation::eye(), Vec3f scale = Vec3f::one, Material m = Material(1, 2))
		: SceneObject(Transform(pos, rot, scale), m)
	{
	}

	// Virtual function overrides
	virtual Vec3f getNormalAt(Vec3f pos) override;
	virtual Color getColorAt(Vec3f pos) override;
	virtual bool findIntersection(Ray& ray, float& dist) override;

	bool loadOBJ(const std::string& fileName);
	void printBuffers();

private:
	void clearBuffers();
};