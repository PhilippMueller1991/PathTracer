#pragma once

#include "Vector.h"

class Camera
{
public:
	int width, height;
	float aspectRatio;
	Vec3f pos, dir, right, up;

	Camera(int width, int height) 
		: pos(Vec3f(0, 0, 0)), dir(Vec3f(0, 0, 1)), right(Vec3f::right), up(Vec3f::up), width(width), height(height)
	{
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}
	Camera(Vec3f pos, Vec3f dir, int width, int height)
		: pos(pos), dir(dir), width(width), height(height)
	{
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		updateDirection(dir);
	}
	~Camera() {}

	void updateDirection(Vec3f dir);
	Vec3f pixelToRayDir(int x, int y, float xSubPixelOffset = 0.5f, float ySubPixelOffset = 0.5f);
};

