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

// Key words: override, final
struct Base {
	virtual void update();
	virtual void foo(int);
	virtual void bar() const;
};
struct Derived : Base {
	void updata() override;					// error - no such a method in Base (updatA)
	void foo(int) override;					// ok    - signatures the same in Base and Derived
	virtual void foo(long) override;		// error - different parm type
	virtual void foo(int) const override;	// error - this method is constant
	virtual int  foo(int) override;			// error - different return value type
	virtual void bar(long);					// ok    - different parm type but not override
	virtual void bar() const final;			// final - it can't be redefined in a derived classe
};
struct Derived2 final : Derived {			// final - Derived2 can't be inherited
	virtual void bar() const;				// error - Derived::bar is final
};
struct Derived3 : Derived2 {};				// error - Derived2 is final