#include "Camera.h"

#include <iostream>

void Camera::UpdateDirection(Vec3f dir)
{
	//dir = dir.Normalize();

	right = static_cast<Vec3f>(Vec3f::up).Cross(dir).Normalize();
	up = dir.Cross(right).Normalize();
}

Vec3f Camera::PixelToRayDir(int x, int y, float xSubPixelOffset, float ySubPixelOffset)
{
	float xOffset = (static_cast<float>(x) + xSubPixelOffset) / static_cast<float>(width);	// Between [0,1]
	xOffset -= 0.5f;
	xOffset *= aspectRatio;
	float yOffset = (static_cast<float>(y) + ySubPixelOffset) / static_cast<float>(height);	// Between [0,1]
	yOffset -= 0.5f;

	return (dir + right * xOffset + up * yOffset).Normalize();	// Pixel center at z=1
}
