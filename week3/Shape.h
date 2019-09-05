#pragma once

struct Shape { };

struct Rectangle : Shape { };

struct Triangle : Shape { };

// Check if two rectangles intersect
bool is_intersect_r_r(Shape* a, Shape* b) {
	return rand() % 2;
}

// Check if a rectangle and a triangle intersect
bool is_intersect_r_t(Shape* a, Shape* b) {
	return rand() % 2;
}