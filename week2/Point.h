#pragma once

struct Point
{
	constexpr Point(double x, double y)
		: x(x), y(y)
	{}

	double x = 0;
	double y = 0;
};

// operator sum
constexpr Point & operator+(Point & lp, Point & rp)
{
	return Point(lp.x + rp.x, lp.y + rp.y);
}

// subtraction
constexpr Point & operator-(Point lp, Point  rp)
{
	return  Point(lp.x - rp.x, lp.y - rp.y);
}

// scalar product
constexpr double operator*(Point lp, Point  rp)
{
	return (lp.x*rp.x + lp.y*rp.y);
}