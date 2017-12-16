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
		return Color(r + rhs.r, g + rhs.g, b + rhs.b).Clamp();
	}
	void operator+=(const Color& rhs)
	{
		*this = *this + rhs;
		this->Clamp();
	}
	Color operator*(float rhs) const
	{
		return Color(rhs * r, rhs * g, rhs * b).Clamp();
	}
	void operator*=(float rhs)
	{
		r *= rhs;
		g *= rhs;
		b *= rhs;
		this->Clamp();
	}
	friend Color operator*(float lhs, const Color& rhs)
	{
		return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b).Clamp();
	}
	Color operator*(const Color& rhs) const
	{
		return Color(r * rhs.r, g * rhs.g, b * rhs.b).Clamp();
	}
	void operator*=(const Color& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		this->Clamp();
	}
	Color operator/(float rhs) const
	{
		return Color(r / rhs, g / rhs, b / rhs).Clamp();
	}
	friend Color operator/(float lhs, const Color& rhs)
	{
		return Color(rhs.r / lhs, rhs.g / lhs, rhs.b / lhs).Clamp();
	}
	void operator/=(float rhs)
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
		this->Clamp();
	}

	friend std::ostream& operator<<(std::ostream& os, const Color& obj)
	{
		os << "Color(" << obj.r << "," << obj.g << "," << obj.b << ")";
		return os;
	}

	Color Clamp() const;
	static inline float Clamp(const float lo, const float hi, const float v);

	// Predefined colors
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color yellow;
	static const Color orange;
};

