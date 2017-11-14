#include "Texture.h"

// UV values are in range(0,1)
Color Texture::GetColorAt(float u, float v)
{
	Color c(0,0,0);
	// TODO
	return c;
}

Color Texture::GetColorAt(Vector3 pos)
{
	Color c(0, 0, 0);
	switch (type)
	{
	case Texture::CHESS_BOARD:
	default:
		bool x = (int)((pos.x) / scale) % 2 == 0;
		bool y = (int)((pos.y) / scale) % 2 == 0;
		bool z = (int)((pos.z) / scale) % 2 == 0;

		if (!(x ^ y ^ z)) 
			c = Color(1, 1, 1);
		break;
	}
	return c;
}