#pragma once

struct CStyleStruct{
	int x;
	double y;
};

struct CPPStyleStruct {
	CPPStyleStruct(int x, double y) : x(x), y(y) {}
	int x;
	double y;
};
