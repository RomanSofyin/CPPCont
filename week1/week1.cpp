// week1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Base.h"
#include "Unit.h"
#include "Expression.h"
#include "Number.h"
#include "BinaryOperation.h"

using namespace std;

// base указывает на экземпляр Base, соответствующий D1
// нужно вернуть указатель на экземпляр Base, соответствующий D2
Base const * D1BaseToD2Base(Base const * base)
{
	return (Base const *)(D2 *)(D3 *)(D1 *)base; // обходим всё дерево наследования
}

bool check_equals(Expression const *left, Expression const *right)
{
	return typeid(*left) == typeid(*right);
}

int* (*ComplexFunction1)(const char *);
/*              |
	   +--------+-----------------------------+
	   |                                      |
	   V                                      V               */
int* (*(*ComplexFunction)(int, int(*)(double)))(const char *);

typedef int * (*cf1)(const char *);
typedef cf1(*cf)(int, int(*)(double));

// возвращает true, если p и q указывают на один и тот же объект
template<class T>
bool isSameObject(T const * p, T const * q)
{
	return dynamic_cast<void const *>(p) == dynamic_cast<void const *>(q);
}
/*
template<class T1, class T2>
bool compare(&T obj1, &T obj2, 
	)
{

}
*/

int main()
{
	cout << "Hi, Awesome Guys!" << endl;

	D1 d1;
	Base const * p_d1 = &d1;
	Base const * d2 = D1BaseToD2Base(p_d1);

	ManBearPig manBearPig(456);

	/* * */
	int a = 27;
	int const b = 412;
	int * pa = &a;
	int const c = const_cast<int const &>(a);
	int d = const_cast<int &>(b);
	int const * p1 = const_cast<int const *>(pa);
	int * const * p2 = const_cast<int * const *>(&pa);
	// T** -> T const ** - запрещено
	int const ** p3 = const_cast<int const **>(&pa);
	int const * const * p4 = const_cast<int const * const *>(&pa);
	/* * */
	cout << typeid(p_d1).name() << endl;

	Number n(3);
	BinaryOperation bo(new Number(567.5), '/', new BinaryOperation(new Number(8.123), '-', new Number(3.25)));
	check_equals(&n, &bo);
	check_equals(&bo, &n);
	check_equals(&n, &n);
	check_equals(&bo, &bo);
	/* * */
	isSameObject<Expression>(&n, &bo);

	return 0;
}

