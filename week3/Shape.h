#pragma once

struct Shape {
	virtual ~Shape() {} // add virtual method to make Shape polymorphic
};

struct Rectangle : Shape { };

struct Triangle : Shape { };

// Check if two rectangles intersect
bool is_intersect_r_r(Shape* a, Shape* b) {
	return rand() % 3;
}

// Check if a rectangle and a triangle intersect
bool is_intersect_r_t(Shape* a, Shape* b) {
	return rand() % 2;
}