// week2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Array.h"


/*
 * static_assert usage
 */
template <class T> void foo(T data, size_t n) {
	static_assert(std::is_signed<T>::value, "T is not signed.");
	int a = 4;
	a = a * 2;
	return;
}

/*
 * New syntax for function declarations
 */
// C++11
template <typename A, typename B>
auto Plus(A a, B b) -> decltype(a + b) {
	return a + b;
}
// C++14
template <typename A, typename B>
auto Pluss(A a, B b) {
	return a + b;
}

/*
 * Variadic function templates.
 *
 * The way the printf function can be implemented.
 */
void usr_printf(char const *s) {
	while (*s) {
		if (*s == '%' && *(++s) != '%')
			// %_control_char_ occured but no more args left, so error handling
			;
		std::cout << *s++;
	}
}
template<typename T, typename... Args>
void usr_printf(char const *s, T value, Args... args) {
	while (*s) {
		if (*s == '%' && *(++s) != '%') {
			// %_control_char_ occured
			std::cout << value;
			usr_printf(++s, args...);
			return;
		}
		std::cout << *s++;
	}
	// no more %_control_char_ left but there are unused args, so error handling
	;
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
	decltype(str[0]) c2 = str[7];			// c2 - (char &)						/* char& operator[] */
	decltype(str[0] + str[7]) c3 = str[1];	// c3 - ( (char &) + (char &) ) = int
	decltype(str) sa = str;					// sa - ( std::string )
	decltype((str)) sb = str;				// sb - ( std::string & )
	c1 = 'K';								// c1 = 'K'
	c2 = '!';								// c2 -> str[7] = '!' i.e. "Compile!"

	auto a = Plus('b', 'c');				// a  - int
	auto d = Plus(5.2, 'f');				// d  - double

	usr_printf("%f, %s\n", 5, "blabla!");

	Array<int> ints1(3);
	Array<int> ints2(ints1);
	size_t s = ints1.size();
	ints2 = std::move(ints1);

	// @ @ @ add unique_ptr usage example here @ @ @


    return 0;
}

