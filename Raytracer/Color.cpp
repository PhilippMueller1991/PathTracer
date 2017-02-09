#include "Color.h"

void Color::Clamp()
{
	float sum = r + g + b;
	*this /= sum;
}