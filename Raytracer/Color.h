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
	Color operator+(const Color& rhs) const
	{
		return Color(r + rhs.r, g + rhs.g, b + rhs.b).clamp();
	}
	void operator+=(const Color& rhs)
	{
		*this = *this + rhs;
		this->clamp();
	}
	Color operator*(float rhs) const
	{
		return Color(rhs * r, rhs * g, rhs * b).clamp();
	}
	void operator*=(float rhs)
	{
		r *= rhs;
		g *= rhs;
		b *= rhs;
		this->clamp();
	}
	friend Color operator*(float lhs, const Color& rhs)
	{
		return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b).clamp();
	}
	Color operator*(const Color& rhs) const
	{
		return Color(r * rhs.r, g * rhs.g, b * rhs.b).clamp();
	}
	void operator*=(const Color& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		this->clamp();
	}
	Color operator/(float rhs) const
	{
		return Color(r / rhs, g / rhs, b / rhs).clamp();
	}
	friend Color operator/(float lhs, const Color& rhs)
	{
		return Color(rhs.r / lhs, rhs.g / lhs, rhs.b / lhs).clamp();
	}
	void operator/=(float rhs)
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
		this->clamp();
	}

	friend std::ostream& operator<<(std::ostream& os, const Color& obj)
	{
		os << "Color(" << obj.r << "," << obj.g << "," << obj.b << ")";
		return os;
	}

	Color clamp() const;
	Color normalize() const;
	static inline float clamp(const float lo, const float hi, const float v);
	static Color getRandomColor();

	// Predefined colors
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color yellow;
	static const Color orange;
};

