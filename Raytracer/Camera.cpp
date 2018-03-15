#include "Camera.h"

#include <iostream>

void Camera::updateDirection(Vec3f dir)
{
	//dir = dir.Normalize();

	right = static_cast<Vec3f>(Vec3f::up).cross(dir).normalize();
	up = dir.cross(right).normalize();
}

Vec3f Camera::pixelToRayDir(int x, int y, float xSubPixelOffset, float ySubPixelOffset)
{
	float xOffset = (static_cast<float>(x) + xSubPixelOffset) / static_cast<float>(width);	// Between [0,1]
	xOffset -= 0.5f;
	xOffset *= aspectRatio;
	float yOffset = (static_cast<float>(y) + ySubPixelOffset) / static_cast<float>(height);	// Between [0,1]
	yOffset -= 0.5f;

	return (dir + right * xOffset + up * yOffset).normalize();	// Pixel center at z=1
}
