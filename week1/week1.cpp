// week1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Base.h"

using namespace std;

// base указывает на экземпляр Base, соответствующий D1
// нужно вернуть указатель на экземпляр Base, соответствующий D2
Base const * D1BaseToD2Base(Base const * base)
{
	return (Base const *)(D2 *)(D3 *)(D1 *)base; // обходим всё дерево наследования
}

int main()
{
	cout << "Hi, Awesome Guys!";

	D1 d1;
	Base const * p_d1 = &d1;
	Base const * d2 = D1BaseToD2Base(p_d1);

	return 0;
}

