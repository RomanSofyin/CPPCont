// week3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Shape.h"
#include "Multimethod2.h"

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

// Usage of iterator_traits::iterator_category
struct random_access_iterator_tag {};
struct bidirectional_iterator_tag {};
struct forward_iterator_tag {};
struct input_iterator_tag {};
struct output_iterator_tag {};
// advance_() version for random access iterators
template<class I>
void advance_(I& i, size_t n, random_access_iterator_tag) {
	i += n;
}
// advance_() version for all other types of iterators
// it has the lowest priority at overloading time
template<class I>
void advance_(I& i, size_t n, ...) {
	for (size_t k = 0; k != n; ++k, ++i);
}
// advance1() chooses proper version of advance_() looking at iterator_category
template<class I>
void advance1(I& i, size_t n) {
	advance_(i, n, typename std::iterator_traits<I>::iterator_category());
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
	
	/*
	 * Multimethod2 task >>>>>>>
	 */
	// multimethod for the heirs of Shape
	// returning bool and commutative
	Multimethod2<Shape, bool, true> is_intersect;
	// add multimethod implementation for two rectangles
	is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);
	// add multimethod implementation for a rectangle and a triangle
	is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);
	// create two shapes
	Shape * t1 = new Triangle();
	Shape * r1 = new Rectangle();

	std::cout << "s1: " << typeid(*t1).name() << std::endl; // Triangle
	std::cout << "s2: " << typeid(*r1).name() << std::endl; // Rectangle

	auto it1 = is_intersect.getImpl(t1, r1);
	auto it2 = is_intersect.getImpl(r1, t1);

	// check that implementation for s1 and s2 exists
	if (is_intersect.hasImpl(t1, r1))
	{
		// is_intersect_r_t(s2, s1) gets called
		bool res = is_intersect.call(t1, r1);
		     res = is_intersect.call(r1, t1);
		// Note: is_intersect_r_t receives
		// a rectangle as the first arg and a triangle as the second
		// here the shapes are passed conversely;
		// implementation must handle this case properly
	}
	/*
	 * Multimethod2 task <<<<<<<
	 */
	
	advance1<decltype(is_intersect.getImpl(t1,r1))>(it1, 1);

}