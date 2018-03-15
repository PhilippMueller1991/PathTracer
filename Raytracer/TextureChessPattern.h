#pragma once

#include "Texture.h"

class TextureChessPattern : public Texture
{
public:
	Color a;
	Color b;

public:
	TextureChessPattern(float scale = 1.0f, Color a = Color::black, Color b = Color::white)
		: Texture(scale), a(a), b(b)
	{
	}
	~TextureChessPattern()
	{
	}

	Color getColorAt(float u, float v) override;
};

