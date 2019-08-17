#pragma once

#include "stdafx.h"
/*
//Move Semantic example

struct String {
	String() = default;
	String(String const & s);
	String & operator=(String const & s);
private:
	char * data_ = nullptr;
	size_t size_ = 0;
};

String getCurrentDataString() {
	String date;
	// date = "21 oct 2015"
	return date;
}

String date = getCurrentDataString();	// redundant copy



// o Constructor Delegation
// o Default values for class fields
// o Inheriting constructors
struct SomeType {
	SomeType(int newNumber) : number(newNumber) {}
	SomeType() : SomeType(42) {}
private:
	int number;
};
struct SomeClass {
	SomeClass() {}
	explicit SomeClass(int newValue) : value(newValue) {}
private:
	int value = 5;
};
struct BaseClass {
	BaseClass(int value);
};
struct DerivedClass : BaseClass {
	using BaseClass::BaseClass;
};
*/