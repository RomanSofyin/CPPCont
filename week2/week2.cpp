// week2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Array.h"

int main()
{
	Array<int> ints1(3);
	Array<int> ints2(ints1);
	size_t s = ints1.size();
	ints2 = std::move(ints1);

	// @ @ @ add unique_ptr usage example here @ @ @


    return 0;
}

