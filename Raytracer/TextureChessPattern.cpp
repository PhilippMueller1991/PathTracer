#include "TextureChessPattern.h"

Color TextureChessPattern::GetColorAt(float u, float v)
{
	bool x = static_cast<int>(u * 2.0f * scale) % 2 == 0;
	bool y = static_cast<int>(v * 2.0f * scale) % 2 == 0;

	if (x ^ y)
		return b;

	return a;
}