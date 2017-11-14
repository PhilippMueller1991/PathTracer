#pragma once

#include "Vector3.h"
#include "Color.h"

// TODO: Generate other texture classes that are derived from this class
// instead of using an enum to generate different texture patterns
// TODO: Generate chessboard class for variable 2 colors
class Texture
{
public:
	enum TextureType { CHESS_BOARD, TEXTURE_TYPE_SIZE};
	TextureType type;
	float scale;
public:
	Texture(TextureType type = CHESS_BOARD, float scale = 1.0f)
		: type(type), scale(scale)
	{
	}
	~Texture()
	{
	}

	Color GetColorAt(float u, float v); // UV values are in range(0,1)
	Color GetColorAt(Vector3 pos);
};

