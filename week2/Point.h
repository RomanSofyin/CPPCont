#pragma once

struct Point
{
	constexpr Point(double x, double y)
		: x(x), y(y)
	{}

	double x = 0;
	double y = 0;
};

// Note: not completely clear why the parm types must be "const Point &" not just "Point"
// MVS 2017 compiles it with no problems with "Point", but stepik doesn't acceps it

// operator sum
constexpr Point operator+(const Point & lp, const Point & rp)
{
	return Point(lp.x + rp.x, lp.y + rp.y);
}

// subtraction
constexpr Point operator-(const Point & lp, const Point &  rp)
{
	return  Point(lp.x - rp.x, lp.y - rp.y);
}

// scalar product
constexpr double operator*(const Point & lp, const Point &  rp)
{
	return (lp.x*rp.x + lp.y*rp.y);
}