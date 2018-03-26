#include "Mesh.h"

Vec3f Mesh::GetNormalAt(Vec3f pos)
{
	// No normal interpolation, instead triangle normal calculation
	const int size = static_cast<int>(nidxs.size());
	int i = 0;
	for (; i < size; i+=3)
	{
		bool isHit = Triangle::IsOnSurface(pos, vertices[vidxs[i]], vertices[vidxs[i + 1]], vertices[vidxs[i + 2]]);
		if (isHit)
		{
			//return -Triangle::getNormal(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
			return (normals[nidxs[i]] + normals[nidxs[i+1]] + normals[nidxs[i+2]]) / 3.0f;
			//return normals[(i+2) / 3];
		}
	}
	return Vec3f::invalid;
}

// TODO
Color Mesh::GetColorAt(Vec3f pos)
{
	return material.GetColorAt(0, 0);
}

bool Mesh::FindIntersection(Ray& ray, float& dist)
{
	dist = -1.0f;
	const int size = static_cast<int>(vidxs.size());
	int i = 0;
	for (; i < size; i += 3)
	{
		bool isHit = Triangle::FindIntersection(ray,
			vertices[vidxs[i + 0]],
			vertices[vidxs[i + 1]],
			vertices[vidxs[i + 2]],
			dist
		);
		if (isHit && dist > 0.0f)
			return true;
	}
	return false;
}

bool Mesh::LoadOBJ(const std::string& fileName)
{
	ClearBuffers();
	return MeshLoader::LoadOBJMesh(fileName, vertices, normals, uvs, vidxs, uvidxs, nidxs);
}

void Mesh::PrintBuffers()
{
	std::cout << "#v " << vertices.size() << '\n';
	std::cout << "#vn " << normals.size() << '\n';
	std::cout << "#uvs " << uvs.size() << '\n';
	std::cout << "#vidxs " << vidxs.size() << '\n';
	std::cout << "#nidxs " << nidxs.size() << '\n';
	for (int i = 0; i < vertices.size(); i++)
	{
		std::cout << "v " << vertices[i] << '\n';
	}
	for (int i = 0; i < uvs.size(); i++)
	{
		std::cout << "vt " << uvs[i] << '\n';
	}
	for (int i = 0; i < normals.size(); i++)
	{
		std::cout << "vn " << normals[i] << '\n';
	}
	for (int i = 0; i < vidxs.size(); i += 3)
	{
		std::cout << "f " 
			<< vidxs[i] + 1		<< '/' << uvidxs[i] + 1		<< '/' << nidxs[i] + 1		<< ", " 
			<< vidxs[i + 1] + 1 << '/' << uvidxs[i + 1] + 1	<< '/' << nidxs[i + 1] + 1	<< ", "
			<< vidxs[i + 2] + 1 << '/' << uvidxs[i + 2] + 1	<< '/' << nidxs[i + 2] + 1	<< '\n';
	}
}

void Mesh::ClearBuffers()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	vidxs.clear();
	nidxs.clear();
}