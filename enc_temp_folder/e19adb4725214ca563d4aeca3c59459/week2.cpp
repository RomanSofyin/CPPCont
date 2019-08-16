// week2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Array.h"



template <class T> void foo(T data, size_t n) {
	static_assert(std::is_signed<T>::value, "T is not signed.");
	int a = 4;
	a = a * 2;
	return;
}

int main()
{
	int intVar = 5;
	size_t sizeVar = 10;
	foo<int>(intVar, 2);
	// foo<size_t>(sizeVar, 5);   <- compile time error with message "T is not signed."

	std::string str = "Compiler";
	auto sz = str.size();					// sz - size_t
	auto c1 = str[0];						// c1 - char
	decltype(str[0]) c2 = str[7];			// c2 - char &
	decltype(str[0] + str[7]) c3 = str[1];	// c3 - ( (char &) + (char &) = int )
	decltype(str) sa = str;
	decltype((str)) sb = str;
	c1 = 'K';
	c2 = '!';

	Array<int> ints1(3);
	Array<int> ints2(ints1);
	size_t s = ints1.size();
	ints2 = std::move(ints1);

	// @ @ @ add unique_ptr usage example here @ @ @


    return 0;
}

