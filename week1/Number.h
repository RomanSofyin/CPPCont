#pragma once

#include "Expression.h"
//#include "Visitor.h"

struct Number : Expression
{
	Number(double value)
		: value(value)
	{}

	virtual void bla() const {}

	double evaluate() const {
		// we can call virtual methods from non-virtual ones
		bla();
		return value;
	}

	//void visit(Visitor * v) const {
	//	v->visitNumber(this);
	//}

	double get_value() const { return value; }

private:
	double value;
};
