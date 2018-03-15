#include "Color.h"
#include <math.h>

const Color Color::white = Color(1, 1, 1);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(1, 0, 0);
const Color Color::green = Color(0, 1, 0);
const Color Color::blue = Color(0, 0, 1);
const Color Color::yellow = Color(1, 1, 0);
const Color Color::orange = Color(1, 0.5f, 0);

Color Color::clamp() const
{
	return Color(clamp(0.0f, 1.0f, r), clamp(0.0f, 1.0f, g), clamp(0.0f, 1.0f, b));
}

float Color::clamp(const float lo, const float hi, const float v)
{
	return fmaxf(lo, fminf(hi, v));
}

Color Color::normalize() const
{
	if (r <= 1.0f && g <= 1.0f && b <= 1.0f)
		return *this;
	float sum = r + g + b;
	return Color(r / sum, g / sum, b / sum);
}

Color Color::getRandomColor()
{
	return Color((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
}