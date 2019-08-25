// week2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Array.h"
#include "Point.h"
#include "StyleStruct.h"


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
 * New syntax for function declarations (late-specified/trailing return type)
 */
// C++11
template <typename A, typename B>
auto Plus(A a, B b) -> decltype(a + b) {
	return a + b;
}
// C++14
template <typename A, typename B>
decltype(auto) Pluss(A a, B b) { // "auto Pluss(A a, B b)" also possible
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

void print_values(std::ostream & os) {}
template<typename T, typename... Args>
void print_values(std::ostream & os, T arg, Args... args) {
	os << typeid(arg).name() << ": " << arg <<'\n';
	print_values(os, args...);   // "args..." is called parameter pack, it includes empty parm pack
}

/*
 * unique_ptr demo
 */
template<typename T> void   unique_ptr_demo_helper1(std::unique_ptr<T> u_ptr) {}
std::unique_ptr<Array<int>> unique_ptr_demo_helper2() {
	std::unique_ptr<Array<int>> ret(new Array<int>(0));
	return ret;
}
/* o unique_ptr is a replacement of auto_ptr in C++11
 * o unique_ptr owns and manages another object through a pointer
 *   and disposes of that object when the unique_ptr goes out of scope
 * o it is obligated to move (rather than copy) managing object
 *   since copy constructor (as well as assignment operator) is restricted in unique_ptr
 */
void unique_ptr_demo() {
	std::unique_ptr<Array<int>> p1(new Array<int>());	// p1 owns created object
	// std::unique_ptr<Array<int>> p2(p1);				// copile time error because it requires copy constructor
														// which is restricted in unique_ptr
	std::unique_ptr<Array<int>> p2(std::move(p1));
	p1 = std::move(p2);
	unique_ptr_demo_helper1(std::move(p1));				// move must be used not to call copy constructor here
														// which is restricted in unique_ptr,
														// so it would result in compile time error
	
	p2 = unique_ptr_demo_helper2();						// here we are not obligated to use anything else
														// since in "p2 = xxx" expression - xxx is a rvalue
}

/*
 * std::tuple demo
 */
std::tuple<std::string, int, int> getUnitInfo(int id) {
	if (id == 0) return std::make_tuple("Elf",   60, 9);
	if (id == 1) return std::make_tuple("Dwarf", 80, 6);
	if (id == 2) return std::make_tuple("Orc",   90, 3);
	return std::make_tuple("", 0, 0);
}
void tuple_demo() {
	auto ui0 = getUnitInfo(0);
	std::cout	<< "race: "	<< std::get<0>(ui0) << ", "		// get<> indexes (0,1,2...) are template parameters
				<< "hp: "	<< std::get<1>(ui0) << ", "		// so they have to be known at compile-time,
				<< "iq: "	<< std::get<2>(ui0) << "\n";	// we can't just go them through in a loop thus

	std::string race1; int hp1; int iq1;
	std::tie(race1, hp1, iq1) = getUnitInfo(1);			// link lvalue references with the tuple fields
	std::cout	<< "race: "	<< race1	<< ", "
				<< "hp: "	<< hp1		<< ", "
				<< "iq: "	<< iq1		<< "\n";
}

/* to_pair()
 * 
 * o I and J along with T template parameters allow to call the func this way: to_pair<I,J>(myTuple)
 *    - I, J    - integer vars identifying tuple fields
 *    - myTuple - a tuple with any number of fields
 * o RETURN_TYPE1 is used here just to increase readability of the code.
 * o "auto ... -> decltype(...)" construction is called "late-specified return type" or "trailing return type"; google for more info
 */
#define RETURN_TYPE1 std::make_pair( std::get<I>(t), std::get<J>(t) )
template<int I, int J, typename... T>
auto to_pair(std::tuple<T...> t) -> decltype(RETURN_TYPE1)
{
	return RETURN_TYPE1;
}

// Range-based for demo
// This kind of for loop can used with any type for which
// begin() and end() are defined
void rangeBasedFor_demo() {
	int rbdArray[] = { 1,4,9,16,25,36,49 };
	int sum = 0;
	// by value
	for (int x : rbdArray) {
		sum += x;
	}
	// by reference
	for (int & x : rbdArray) {
		x *= 2;
	}
}

// Initializer list demo
// std::initializer_list can be used for functions and methods
int initListSum_demo(std::initializer_list<int> list) {
	int result = 0;
	for (int x : list)
		result += x;
	return result;
}

// Uniform Initialization ( >= C++11 )
CStyleStruct getValue() {
	return { 6, 4.2 };	// return type can be omited - compiler knows it from the func declaration
}
void uniformInit_demo() {
	// C++03
	CStyleStruct   s1 = { 19, 72.0 };
	CPPStyleStruct s2(19, 83.0);
	// C++11
	CStyleStruct   s3{ 19, 72.0 };
	CPPStyleStruct s4{ 19, 83.0 };
	CStyleStruct   s5 = getValue();
}

// std::function demo
// std::function object can point to:
//   - function
//   - method
//   - functor (functional object)
int mult(int x, int y) { return x * y; }
struct IntDiv {
	int operator()(int x, int y) const {
		return x / y;
	}
};
int stdFunction_demo(const char & o, int a, int b) {
	std::function<int (int, int)> op;
	if (o == '*')
		op = &mult;
	else if (o == '/')
		op = IntDiv();
	return  op(a, b);
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

	print_values(std::cout, 0, 3.5, "Hello");

	Array<int> ints1(3);
	Array<int> ints2(ints1);
	size_t s = ints1.size();
	ints2 = std::move(ints1);

	unique_ptr_demo();

	tuple_demo();

	auto t = std::make_tuple(0, 3.5, "Hello");
	std::pair<double, char const*> p = to_pair<1, 2>(t);	// 1 and 2 - are the template parameters I and J

	constexpr Point  cp1(1, 2), cp2(3, 4), cp3(5, 6), cp4(7, 8);
	constexpr size_t size = static_cast<size_t>(Point(2, 4) * Point(4, 3));
	int m[size];											// m holds 20 elements
	          Point    p23   =  cp2  + cp3;
	constexpr Point   cp23   =  cp2  + cp3;
	          Point    p12   =  cp1  - cp2;
	constexpr Point   cp12   =  cp1  - cp2;
	          double   p2312 =  cp23 * cp12;
	constexpr double  cp2312 =  cp23 * cp12;
	                   p2312 = (cp2  + cp3) * (cp1 - cp2);
	constexpr double ncp2312 = (cp2  + cp3) * (cp1 - cp2);	// it must be a separate constexpr constant
															// because we can't reassign constexpr constants

	rangeBasedFor_demo();

	auto s1 = initListSum_demo({ 1,1,2,3,5,8,13,21 });

	uniformInit_demo();

	auto res = stdFunction_demo('/', 123, 5);
	
	return 0;
}

