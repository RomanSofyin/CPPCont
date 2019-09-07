// week3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Shape.h"
//#include "Multimethod2.h"

// std::list demo
void listDemo() {
	using std::string;
	std::list<string> list = { "One", "Two", "Three" };
	
	// iterate over all elements
	for (string & s : list)
		std::cout << s << std::endl;
	
	// using iterators
	std::list<string>::iterator i = list.begin();
	for (; i != list.end(); i++) {	// ++i and i++ have no differences for iterators?
		if (*i == "Two")
			break;
	}
	list.erase(i);
}

template <class It>
size_t max_increasing_len(It p, It q) {
	size_t res = 0, len = 0;
	It i = p;
	while (p != q) {
		if (*i < *p) {
			len++;
		}
		else {
			res = std::max(res, len);
			len = 1;
		}
		i = p++;
	}
	res = std::max(res, len);
	return res;
}

int main()
{
	listDemo();

	std::list<int> const l0 = { };
	size_t len0 = max_increasing_len(l0.begin(), l0.end());
	std::list<int> const l1 = { 7,8,9,4,5,6,1,2,3,4 };
	size_t len1 = max_increasing_len(l1.begin(), l1.end()); // 4 coresponding to "1,2,3,4"
	std::list<int> const l2 = { -3,-2,-1,0,0,1,2,3,4,5 };
	size_t len2 = max_increasing_len(l2.begin(), l2.end()); // 6 coresponding to "0,1,2,3,4,5"
	
	auto tmp1 = std::type_index(/* const type_info & */typeid(Shape));

	// multimethod for the heirs of Shape
	// returning bool and commutative
	Multimethod2<Shape, bool, true> is_intersect;
	// add multimethod implementation for two rectangles
	is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);
	// add multimethod implementation for a rectangle and a triangle
	is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);
	// create two shapes
	Shape * s1 = new Triangle();
	Shape * s2 = new Rectangle();
	// check that implementation for s1 and s2 exists
	if (is_intersect.hasImpl(s1, s2))
	{
		// is_intersect_r_t(s2, s1) gets called
		bool res = is_intersect.call(s1, s2);
		// Note: is_intersect_r_t receives
		// a rectangle as the first arg and a triangle as the second
		// here the shapes are passed conversely;
		// implementation must handle this case properly
	}
}