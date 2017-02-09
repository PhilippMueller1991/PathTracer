#include "Color.h"

const Color Color::white(1, 1, 1);
const Color Color::black(0, 0, 0);
const Color Color::red(1, 0, 0);
const Color Color::green(0, 1, 0);
const Color Color::blue(0, 0, 1);
const Color Color::yellow(1, 0, 1);

void Color::Clamp()
{
	float sum = r + g + b;
	*this /= sum;
}