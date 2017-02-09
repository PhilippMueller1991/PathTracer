#include "Camera.h"

#include <iostream>

void Camera::UpdateDirection(Vector3 dir)
{
	dir = dir.Normalize();

	right = static_cast<Vector3>(Vector3::up).Cross(dir).Normalize();
	up = dir.Cross(right).Normalize();
}

Vector3 Camera::PixelToRayDir(int x, int y, const int width, const int height)
{
	const float xSubPixelOffset = 0.5f;	// Range(0,1)
	const float ySubPixelOffset = 0.5f; // Range(0,1)

	float xOffset = (static_cast<float>(x) + xSubPixelOffset) / static_cast<float>(width);	// Between [0,1]
	xOffset -= 0.5f;
	float yOffset = (static_cast<float>(y) + ySubPixelOffset) / static_cast<float>(height);	// Between [0,1]
	yOffset -= 0.5f;

	return (dir + right * xOffset + up * yOffset).Normalize();	// Pixel center at z=1
}
