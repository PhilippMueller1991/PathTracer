#include "Texture.h"

// UV values are in range(0,1)
Color Texture::GetColorAt(float u, float v)
{
	Color c(0, 0, 0);

	bool x = (u * scale - floorf(u * scale)) < 0.5f;
	bool y = (v * scale - floorf(v * scale)) < 0.5f;

	if (x ^ y)
		c = Color(1, 1, 1);

	return c;
}