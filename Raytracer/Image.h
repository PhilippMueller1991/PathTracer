#pragma once

#include "Color.h"
#include <string>

struct RGB {
public:
	// RBG values have to be normalized
	float r;
	float g;
	float b;

	RGB() {}
	RGB(float x, float y, float z) : r(x), g(y), b(z) {}
};

class Image
{
public:
	int width, height, dpi;
	RGB* data;

	Image(const int width, const int height, const int dpi = 72) 
		: width(width), height(height), dpi(dpi)
	{
		data = new RGB[width * height];
	}
	~Image() 
	{
		delete data;
	}

	void setPixel(int idx, Color color);
	void saveBitmap(std::string filename);
};

