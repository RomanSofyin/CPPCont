// week3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

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
	size_t res = 0;
	for (It i = p, j = p; p != q;) {
		if (*j < *(++p))
			j = p;
		else {
			size_t d   = std::distance(i, p);
			res = std::max(res, d);
			i = j = p;
		}
	}

	return res;
}

int main()
{
	listDemo();

	std::list<int> const l1 = { 7,8,9,4,5,6,1,2,3,4 };
	size_t len1 = max_increasing_len(l1.begin(), l1.end()); // 4 coresponding to "1,2,3,4"
	std::list<int> const l2 = { -3,-2,-1,0,0,1,2,3,4,5 };
	//auto iterator = l2.begin();
	//iterator.operator->
	size_t len2 = max_increasing_len(l2.begin(), l2.end()); // 6 coresponding to "0,1,2,3,4,5"
}