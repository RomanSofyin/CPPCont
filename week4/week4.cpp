#include "pch.h"
#include "bad_from_string.h"

using namespace std;

template<class T>
T from_string(string const& s)
{
	T v;
	istringstream iss(s);
	iss.setf(std::ios_base::skipws);
	iss >> v;
	return v;
}

void from_string_test() {
	string s1("123");
	int    a1 = from_string<int>(s1); // a1 = 123
	double b1 = from_string<double>(s1); // b1 = 123.0
	string c1 = from_string<string>(s1); // c1 = "123"

	string s2("12.3");
	try {
		int    a2 = from_string<int>(s2); // исключение
	}
	catch (bad_from_string & e) {
		cout << "An exception has been thrown by from_string<int>(s2)\n";
	}
	double b2 = from_string<double>(s2); // b2 = 12.3
	string c2 = from_string<string>(s2); // c2 = "12.3"

	string s3("abc");
	try {
		int    a3 = from_string<int>(s3); // исключение
	}
	catch (bad_from_string & e) {
		cout << "An exception has been thrown by from_string<int>(s3)\n";
	}
	try {
		double b3 = from_string<double>(s3); // исключение
	}
	catch (bad_from_string & e) {
		cout << "An exception has been thrown by from_string<double>(s3)\n";
	}
	string c3 = from_string<string>(s3); // c3 = "abc"
}

int main()
{
	from_string_test();
}