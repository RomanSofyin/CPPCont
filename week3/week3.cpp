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
}