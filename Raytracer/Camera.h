#pragma once

#include "Vector3.h"

class Camera
{
public:
	Vector3 pos, dir, right, up;

	Camera() : pos(Vector3(0, 0, 0)), dir(Vector3(0, 0, 1)), right(Vector3::right), up(Vector3::up) {}
	Camera(Vector3 pos, Vector3 dir) : pos(pos), dir(dir)
	{
		UpdateDirection(dir);
	}
	~Camera() {}

	void UpdateDirection(Vector3 dir);
	Vector3 PixelToRayDir(int x, int y, const int width, const int height);
};

