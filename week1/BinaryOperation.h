#pragma once

#include "Expression.h"
//#include "Visitor.h"

struct BinaryOperation : Expression
{
	//void visit(Visitor * v) const {
	//	v->visitBinaryOperation(this);
	//}

	BinaryOperation(Expression const * left, char op, Expression const * right)
		: left(left), op(op), right(right)
	{}

	~BinaryOperation() {
		delete left;
		delete right;
	}

	double evaluate() const {
		double res = 0;

		switch (op)
		{
		case '+':
			res = left->evaluate() + right->evaluate();
			break;
		case '-':
			res = left->evaluate() - right->evaluate();
			break;
		case '*':
			res = left->evaluate() * right->evaluate();
			break;
		case '/':
			res = left->evaluate() / right->evaluate();
			break;
		default:
			break;
		}
		return res;
	}

	Expression const * get_left() const { return left; }
	Expression const * get_right() const { return right; }
	char get_op() const { return op; }

private:
	Expression const * left;
	Expression const * right;
	char op;
};