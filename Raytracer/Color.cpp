#include "Color.h"
#include <math.h>

const Color Color::white = Color(1, 1, 1);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(1, 0, 0);
const Color Color::green = Color(0, 1, 0);
const Color Color::blue = Color(0, 0, 1);
const Color Color::yellow = Color(1, 0, 1);

Color Color::Clamp()
{
	//float sum = r + g + b;
	//return *this / sum;
	return Color(Clamp(0.0f, 1.0f, r), Clamp(0.0f, 1.0f, g), Clamp(0.0f, 1.0f, b));
}

float Color::Clamp(const float lo, const float hi, const float v)
{
	return fmaxf(lo, fminf(hi, v));
}