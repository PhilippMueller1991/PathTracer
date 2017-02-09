#pragma once

#include <iostream>

class Color
{
public:
	float r, g, b;

	Color() : r(0), g(0), b(0) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
	~Color() {}

	// Operation overload
	Color operator*(float rhs) const
	{
		return Color(rhs * r, rhs * g, rhs * b);
	}
	friend Color operator*(float lhs, const Color& rhs)
	{
		return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b);
	}
	void operator*=(float rhs)
	{
		r *= rhs;
		g *= rhs;
		b *= rhs;
	}
	Color operator/(float rhs) const
	{
		return Color(r / rhs, g / rhs, b / rhs);
	}
	friend Color operator/(float lhs, const Color& rhs)
	{
		return Color(rhs.r / lhs, rhs.g / lhs, rhs.b / lhs);
	}
	void operator/=(float rhs)
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
	}

	friend std::ostream& operator<<(std::ostream& os, const Color& obj)
	{
		os << "Color(" << obj.r << "," << obj.g << "," << obj.b << ")";
		return os;
	}

	void Clamp();
};

