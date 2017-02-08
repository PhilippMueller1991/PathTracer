#pragma once

class Color
{
public:
	float r, g, b, a;

	Color() : r(0), g(0), b(0), a(1) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	~Color() {}

	// Operation overload
	Color operator*(float rhs) const
	{
		return Color(rhs * r, rhs * g, rhs * b, rhs * a);
	}
	friend Color operator*(float lhs, const Color& rhs)
	{
		return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
	}
	void operator*=(float rhs)
	{
		r *= rhs;
		g *= rhs;
		b *= rhs;
		a *= rhs;
	}
	Color operator/(float rhs) const
	{
		return Color(r / rhs, g / rhs, b / rhs, a / rhs);
	}
	friend Color operator/(float lhs, const Color& rhs)
	{
		return Color(rhs.r / lhs, rhs.g / lhs, rhs.b / lhs, rhs.a / lhs);
	}
	void operator/=(float rhs)
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
		a /= rhs;
	}
};

