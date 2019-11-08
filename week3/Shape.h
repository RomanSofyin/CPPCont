#pragma once

struct Shape {
	virtual ~Shape() {} // add virtual method to make Shape polymorphic
};

struct Rectangle : Shape { };

struct Triangle : Shape { };

// Check if two rectangles intersect
bool is_intersect_r_r(Shape* a, Shape* b) {
	return rand() % 2;
}

// Check if two triangles intersect
bool is_intersect_t_t(Shape* a, Shape* b) {
    return rand() % 3;
}

// Check if a rectangle and a triangle intersect
bool is_intersect_r_t(Shape* a, Shape* b) {
    std::string aType = typeid(*a).name();
    std::string bType = typeid(*b).name();
    if (aType != "struct Rectangle" || bType != "struct Triangle") {
        std::cout << "is_intersect_r_t gets called with wrong parms" << std::endl;
        int zero = 0;
        int asd = 4 / zero;
    }
    return rand() % 4;
}