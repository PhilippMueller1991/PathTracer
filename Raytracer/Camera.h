#pragma once

#include "Vector3.h"

class Camera
{
public:
	int width, height;
	float aspectRatio;
	Vector3 pos, dir, right, up;

	Camera(int width, int height) 
		: pos(Vector3(0, 0, 0)), dir(Vector3(0, 0, 1)), right(Vector3::right), up(Vector3::up), width(width), height(height)
	{
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}
	Camera(Vector3 pos, Vector3 dir, int width, int height)
		: pos(pos), dir(dir), width(width), height(height)
	{
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		UpdateDirection(dir);
	}
	~Camera() {}

	void UpdateDirection(Vector3 dir);
	Vector3 PixelToRayDir(int x, int y, float xSubPixelOffset = 0.5f, float ySubPixelOffset = 0.5f);
};

