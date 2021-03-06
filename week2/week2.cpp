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
 * o "auto ... -> decltype(...)" construction is called "late-specified return type" or "trailing return type"
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

// Uniform Initialization (C++11)
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

// Lambda demo
void lambda_demo() {
	// Define op pointing to a functor which is defined via lambda 
	std::function<int(int, int)> op = [](int x, int y)        {return x / y; };
	//                                  **************                         - parameter list
	//                                                        **************** - lambda body
	// the same as above but specifying return type
	                             op = [](int x, int y) -> int {return x / y; };
	//                                                 ******                  - trailing-return-type
	// Template functor (C++14)
								 op = [](auto x, auto y)      {return x / y; };
	
	// It's possible to capture a local variable into the lambda function;
	// addToTotal POINTS TO A FUNCTOR which adds the parm value to the total local variable
	int total = 0;
	auto addToTotal = [&total](int x)   {total += x; };
	//                ********                                                 - capture clause (aka lambda-introducer)
	auto addTotal   = [total] (int & x) {x -= total; };
	// it's possible to capture "this" if we are inside of method
	// auto callUpdate = [this](){ this->update(); };
	
	// Labda usage example
	addToTotal(5);	// total = 5
	addToTotal(5);	// total = 10
	addToTotal(5);	// total = 15
}

/* Lambda capture clauses demo
 *  o []        - nothing will be captured
 *  o [x, &y]   - capture x by value, y by reference
 *  o [&]       - all the vars mentioned in the lambda body will be captured by reference
 *  o [=]       - . . . by value
 *  o [&, x]    - . . . by reference except x which will be captrued by value
 *  o [=, &z]   - . . . by value except z which will be captrued by reference
 *  o [x = std::move(y)] - capure with movement (value of the x will be moved to the lambda local var x, original x var may be removed/cleared/...) (C++14)
 */
// General recomendation - do not use capture types [&] and [=]
// Below is described why
struct Checker {
	std::function<bool(int)> getFilter() const {
		// it captures this, not a local variable d
		return [=](int x) {return x % d == 0; };	// we are returning a functor which referes to this->d,
													// so it's possible that obj *this will be destroyed (and consequently d won't exist)
													// when the functor will be called, it will refer to d var and this is the problem
	}
	auto getModulo() const { return 0; }
	int d;
};
std::function<bool(int)> getFilter(Checker const& c) {
	auto d = c.getModulo();
	//it captures a reference to a local var d
	return [&] (int i) {return i % d == 0; };		// similar problem - d is local variable of getFilter(), functor uses d and is returned out of getFilter().
													// so, the functor can be called when d won't exist (because it's local var of getFilter())
}

template<class F>
void for_each_int(int * p, int * q, F f)
{
	for (; p != q; ++p)
		f(*p);
}

template<class F>
int* find_if(int* p, int* q, F f)
{
	for (; p != q; ++p)
		if (f(*p))
			return p;
	return q;
}

// String literals demo
void stringLiterals_demo() {
	u8"This is a UTF-8 string";		// char[]
	u"This is a UTF-16 string";		// char_16_t[]
	U"This is a UTF-32 string";		// char_32_t[]
	L"This is a wide-char string";	// wchar_t[]
	
	u8"This is a Unicode Character: \u2018.";
	u"This is a bigger Unicode Character: \u2018.";
	U"This is a Unicode Character: \U00002018.";
	
	R"(The String Data \ Stuff " )";
	R"delimiter(The String Data \ Stuff " )delimiter";

	LR"(Raw wide string literal \t (without a tab))";
	u8R"XXX(I'm a "raw UTF-8" string.)XXX";
	uR"*(This is a "raw UTF-16" string.)*";
	UR"(This is a "raw UTF-32" string.)";
}

// Class enum
enum class COLORS
{
	BLACK, WHITE, RED,
	GREEN = 4, BLUE = 8
};
enum class DAYS : unsigned int
{
	MONDAY, TUESDAY, WEDNESDAY, THURSDAY,
	FRIDAY, SATURDAY, SUNDAY
};
void callEnumUsage_demo(DAYS d) {
	switch (d)
	{
	case DAYS::MONDAY:
		break;
	case DAYS::TUESDAY:
		break;
	// . . .
	default:
		break;
	}
}

// In case if template param defined as a lvalue/rvalue reference,
// resulted type of the parm depends on type passed to template.
// The table below shows how it works:
//
//     *defined type*	*passed type*	->	*resulted type*
// (1) T&				MyClass&		->	MyClass&
// (2) T&				MyClass&&		->	MyClass&
// (3) T&&				MyClass&		->	MyClass&
// (4) T&&				MyClass&&		->	MyClass&&
//
// It allows to define universal reference - it can be as lvalue as rvalue reference depending on the type of arg during instantiation
template<typename T>
void foo1(T && t) {}					// T && - is a universal reference
void unitRef_demo() {
	Point & lref_p = Point(1,2.3);
	foo1<Point&>(lref_p);				// -> Point&	- see case (2)
	foo1<Point&&>(std::move(lref_p));	// -> Point&&	- see case (4)
	// foo1<Point>(lref_p);				// compile error - clear
	foo1<Point>(std::move(lref_p));		// gets compiled - not clear !
	foo1<Point&>(std::move(lref_p));	// gets compiled - not clear !
	// foo1<Point&&>(lref_p);			// compile error - clear

}

// How std::move() works?
//  o "std::remove_reference<T>::type"		- removes reference from T type (i.e. T& -> T, T&& -> T)
//  o "std::remove_reference<T>::type&&"	- makes the return type rvalue reference
//  o "typedef typename remove_reference<T>::type&& RvalRef;" - defines an alternative name for retrun type
//  o 'a' - is a local variable, so in the std:move() body it becomes lvalue reference (@001)
// Conclusion: std::move doesn't have executable code, it is compile-time function
template<class T>
typename std::remove_reference<T>::type&& move(T&& a)
{
	typedef typename remove_reference<T>::type&& RvalRef;
	return static_cast<RvalRef>(a);	// transform lvalue reference to rvalue reference
}

// Perfect forwarding demo - std::forward()
// Allows passing an lvalue/rvalue argument to the inner function (T() in our case) without changing its type;
// In other words:
//  o in case Arg is an lvalue reference, arg is passed to T(Arg) constructor as an lvalue reference
//  o in case Arg is an rvalue reference, arg is passed to T(Arg) constructor as an rvalue reference
// It is necessary to use std::forward() because rvalue reference passed to a function becomes an lvalue reference
// since it is local variable within the make_unique() function body (the same case as in @001)
template<typename T, typename Arg>
std::unique_ptr<T> make_unique(Arg&& arg) {
	return std::unique_ptr<T>(
		new T(std::forward<Arg>(arg)));
}

// How std::forward() works?
// Rely on std::move() description and it should become clear.
// std::forward() returns:
//  o A&& - if A or A&& specified as a tempalte parm
//  o A&  - if A& specified as a tempalte parm
template<class S>
S&& forward(typename std::remove_reference<S>::type& a)
{
	return static_cast<S&&>(a);
}

// Variadic template version of make_unique()
template<typename T, typename ...Args>				// list of types
std::unique_ptr<T> make_unique(Args&& ... args) {	// list of args
	return std::unique_ptr<T>(
		new T(std::forward<Args>(args)...)			// list of pairs Argi argi with an expresion to the left of "...", i.e.: forward<Arg1>(arg1) , ... , forward<ArgN>(argN)
		);
}

// Variadic template function apply() using perfect forwarding technique
#define RETURN_TYPE2 f(std::forward<Args>(args)...)
template<typename F, typename ...Args>
auto apply(F f, Args&& ... args ) -> decltype(RETURN_TYPE2) {	// "Args&& ... args" is a variadic universal reference
	return f(std::forward<Args>(args)...);
}

// Conclusion.
// Use std::move() when you want 
//  o to pass a variable as rvalue-reference:
//      X x;
//      foo(std::move(x));
//  o to pass an rvalue-reference which is a func arg to another func:
//      void foo(X&& x) { bar(std::move(x)) }
//  
// Use std::forward() when you want to forward a func arg which is universal reference to another func:
//      template<class T>
//      void foo(T&& t) { bar(std::forward<T>(t)); }

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
	
	lambda_demo();

	int m1[10] = { 1,2,3,4,5,6,7,8,9,10 };
	auto square_fun = [](auto & x) { x *= x; };
	for_each_int(m1, m1 + 10, square_fun); // m = {1,4,9,16,25,36,49,64,81,100};

	int primes[5] = { 2,3,5,7,11 };
	int m2[10] = { 0,0,1,1,4,6,7,8,9,10 };
	auto gen_finder = [](const int * p1, const int * p2) {
		return [=](const int v) {
			for (int i = 0; p1 + i < p2; i++)
				if (p1[i] == v)
					return true;
			return false;
		};
	};
	// first_prime will point to an integer value 7
	int* first_prime = find_if(m2, m2 + 10, gen_finder(primes, primes + 5));

	stringLiterals_demo();

	callEnumUsage_demo(DAYS::SATURDAY);
	// callEnumUsage_demo(COLORS::GREEN); - compile error, wrong arg type

	unitRef_demo();

	auto fun = [](std::string a, std::string const& b) { return a += b; };
	std::string str1("world!");
	// s - is passed by lvalue reference
	// "std::string("Hello, ")" - is a temporary obj, so passed by rvalue reference
	str1 = apply(fun, std::string("Hello, "), str1);	// apply() accepts as many args as fun does

	return 0;
}

