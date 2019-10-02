// week3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Shape.h"
#include "Multimethod2.h"
#include "VectorList.h"
#include "Range.h"
#include "ElementN.h"

// std::list demo
void listDemo() {
	using std::string;
	std::list<string> list = { "One", "Two", "Three" };
	
	// iterate over all elements
	for (string & s : list)
		std::cout << s << std::endl;
	
	// using iterators
	std::list<string>::iterator i = list.begin();
	for (; i != list.end(); i++) {	// ++i and i++ have no differences for iterators?
		if (*i == "Two")
			break;
	}
	list.erase(i);
}

template <class It>
size_t max_increasing_len(It p, It q) {
	size_t res = 0, len = 0;
	It i = p;
	while (p != q) {
		if (*i < *p) {
			len++;
		}
		else {
			res = std::max(res, len);
			len = 1;
		}
		i = p++;
	}
	res = std::max(res, len);
	return res;
}

// Usage of iterator_traits::iterator_category
struct random_access_iterator_tag {};
struct bidirectional_iterator_tag {};
struct forward_iterator_tag {};
struct input_iterator_tag {};
struct output_iterator_tag {};
// advance_() version for random access iterators
template<class I>
void advance_(I& i, size_t n, random_access_iterator_tag) {
	i += n;
}
// advance_() version for all other types of iterators
// it has the lowest priority at overloading time
template<class I>
void advance_(I& i, size_t n, ...) {
	for (size_t k = 0; k != n; ++k, ++i);
}
// advance1() chooses proper version of advance_() looking at iterator_category
template<class I>
void advance1(I& i, size_t n) {
	advance_(i, n, typename std::iterator_traits<I>::iterator_category());
}

// reverse iterator demo
void reverseIterator_demo() {
	std::list<int> l = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	// list<int>::reverse_iterator
	for (auto i = l.rbegin(); i != l.rend(); ++i)
		std::cout << *i << std::endl;
	// iterator <-> reverse_iterator conversion
	std::list<int>::iterator i = l.begin();
	advance(i, 5); // i -> 5
	std::list<int>::reverse_iterator ri(i);	// iterator -> reverse_iterator; ri -> 4
	i = ri.base();							// reverse_iterator -> iterator; i  -> 5
}

void rangeClass_demo() {
	// std::find requires an input iterator
	auto range = Range<15, 25>();
	auto itr = std::find(range.begin(), range.end(), 18);
	std::cout << *itr << '\n'; // 18

	// Range::iterator also satisfies range-based for requirements
	for (long l : Range<3, 5>()) {
		std::cout << l << ' '; // 3 4 5
	}
	std::cout << '\n';
}

void vectorList_test() {
	VectorList<char> vlist;

	{	// do some checks for an empty VectorList instance
		auto v_size = vlist.size();
		auto i = vlist.begin();
		auto d = std::distance(vlist.begin(), vlist.end());

		std::cout << "Size is " << v_size << std::endl;
		//std::cout << "begin is " << *i << std::endl;									// I don't know if I need to allow this for vlist having empty vector
		std::cout << "std::distance(begin,end) " << d << std::endl;
		//std::cout << "*(++begin) == 'B'? " << (*++vlist.begin() == 'B') << std::endl;
		//std::cout << "*(++begin) == 'A'? " << (*++vlist.begin() == 'A') << std::endl;
		std::cout << std::endl;
	}

	std::vector<char> v1;
	v1.push_back('A');
	v1.push_back('B');
	v1.push_back('C');

	std::vector<char> v2;
	v2.push_back('D');
	v2.push_back('E');
	v2.push_back('F');
	v2.push_back('G');

	std::vector<char> v3;

	vlist.append(v1.begin(), v1.end());
	vlist.append(v2.begin(), v2.end());
	vlist.append(v3.begin(), v3.end());	// v3 не будет добавлен, поскольку пустой

	auto v_size = vlist.size();
	auto i = vlist.begin();
	auto d = std::distance(vlist.begin(), vlist.end());

	std::cout << "Size is " << v_size << std::endl;
	std::cout << "begin is " << *i << std::endl;
	std::cout << "std::distance(begin,end) " << d << std::endl;
	std::cout << "*(++begin) == 'B'? " << (*++vlist.begin() == 'B') << std::endl;
	std::cout << "*(++begin) == 'A'? " << (*++vlist.begin() == 'A') << std::endl;
	std::cout << std::endl;

	std::cout << "Test ++i" << std::endl;
	for (i = vlist.begin(); i != vlist.end(); ++i)
		std::cout << *i << " ";
	std::cout << std::endl << "Test i++" << std::endl;
	for (i = vlist.begin(); i != vlist.end(); i++)
		std::cout << *i << " ";
	std::cout << std::endl << "Test --i" << std::endl;
	for (i = vlist.end(); i != vlist.begin();)
		std::cout << *--i << " ";
	std::cout << std::endl << "Test i--" << std::endl;
	for (i = vlist.end(); i != vlist.begin();) {
		i--;
		std::cout << *i << " ";
	}
	std::cout << std::endl << std::endl;

	auto j = vlist.rbegin();
	std::cout << "rbegin is " << *j << std::endl;
	j = --vlist.rend();
	std::cout << "--rend is " << *j << std::endl;

	std::cout << "Test reverse_const_iterator ++" << std::endl;
	for (j = vlist.rbegin(); j != vlist.rend(); ++j)
		std::cout << *j << " ";
	std::cout << std::endl;
	// base() check
	typename VectorList<char>::const_iterator it1 = vlist.begin();
	typename VectorList<char>::const_reverse_iterator rit(it1);
	typename VectorList<char>::const_iterator it2 = rit.base();
	if (it1 == it2)
		std::cout << "base() works fine" << std::endl;
	else
		std::cout << "base() doesn't work as expected" << std::endl;

	return;
}

void algorithms_demo() {
	{
		std::vector<int> v = { 2,3,5,7,13,17,19 };
		size_t c = count_if(v.begin(), v.end(),
			[](int x) {return x % 2 == 0; });
		auto it1 = lower_bound(v.begin(), v.end(), 11);
		bool has7 = binary_search(v.begin(), v.end(), 7);

		std::vector<std::string> db;
		db.push_back("qwe"); db.push_back("qew"); db.push_back("ewq");
		db.push_back("weq"); db.push_back("eqw"); db.push_back("wqe");
		db.push_back("Waldo"); db.push_back("Bob");
		for_each(db.begin(), db.begin() + db.size() / 2,
			[](std::string & s) {std::cout << s << "\n"; });
		auto w = find(db.begin(), db.end(), "Waldo");
		std::string agents[3] = { "Alice", "Bob", "Eve" };
		auto it2 = find_first_of(db.begin(), db.end(),
			agents, agents + 3);
	}

	{
		// случайныe
		std::vector<int> a(100);
		generate(a.begin(), a.end(), []() {return rand() % 100; });
		// 0,1,2,3,...
		std::vector<int> b(a.size());
		iota(b.begin(), b.end(), 0);
		// c[i] = a[i] * b[i]
		std::vector<int> c(b.size());
		transform(
			a.begin(), a.end(),						// границы послед-ти, содержащей первые операнды для функтора multiplies
			b.begin(),								// начало  послед-ти, содержащей вторые операнды для функтора multiplies
			c.begin(),								// начало  послед-ти, в которую положим рез-т        функтора multiplies
			std::multiplies<int>());				// функтор-операция, которую выполняем над эл-ами последовательностей, содержащих операнды
		// c[i] *= 2
		transform(
			c.begin(), c.end(),						// границы послед-ти, содержащей первые (и единственные) операнды функтора
			c.begin(),								// начало  послед-ти, в которую положим рез-т функтора
			[](int x) {return x * 2; }				// функтор-операция
		);
		// сумма c[i]
		int sum = accumulate(c.begin(), c.end(), 0);
	}

	{
		std::vector<int> v1 = { 2,5,1,5,8,5,2,5,8 };
		remove(v1.begin(), v1.end(), 5);				// v.size() не изменится, v = {2,1,8,2,8,5,2,5,8}
		// удаление элемента по значению
		std::vector<int> v2 = { 2,5,1,5,8,5,2,5,8 };
		v2.erase(remove(v2.begin(), v2.end(), 5), v2.end());
		// для std::list можно так
		std::list<int> l1 = { 2,5,1,5,8,5,2,5,8 };
		l1.remove(5);
		
		// Удаление одинаковых элементов
		std::vector<int> v3 = { 1,2,2,2,3,4,5,5,5,6,7,8,9 };
		v3.erase(unique(v3.begin(), v3.end()), v3.end());
		// для std::list можно так
		std::list<int> l2 = { 1,2,2,2,3,4,5,5,5,6,7,8,9 };
		l2.unique();
	}

	// Удаление из ассоциативных контейнеров
	{
		// так не нужно делать
		std::map<std::string, int> m;
		for (auto it = m.begin(); it != m.end(); ++it)
			if (it->second == 0)
				m.erase(it);		// 'it' gets invalidated here, so ++it leads to unpredictable  behaviour
		// нужно делать так
		for (auto it = m.begin(); it != m.end(); )
			if (it->second == 0)
				it = m.erase(it);	// альтернативный вариант: m.erase(it++)
			else
				++it;
	}
}

// алгоритм должен работать с forward итераторами
// и возвращать итератор, который потом будет передан
// в метод erase соответствующего контейнера
template<class FwdIt>
FwdIt remove_nth(FwdIt p, FwdIt q, size_t n)
{
	// std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9,10 };
	// auto newEndIt = remove_nth(v.begin(), v.end(), 5);
	// v.erase(newEndIt, v.end());
	// теперь в v = {0,1,2,3,4,6,7,8,9,10};
	auto d = std::distance(p, q);
	if (n >= d)
		return q;
	auto afterNIt = std::copy_n(p, n, p);
	auto nIt = afterNIt++;
	return std::copy(afterNIt, q, nIt);
}

void remove_nth_test() {
	{
		std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9,10 };
		v.erase(remove_nth(v.begin(), v.end(), 5), v.end());
		// теперь в v = {0,1,2,3,4,6,7,8,9,10};
	}
	{
		std::vector<int> v = { 0 };
		v.erase(remove_nth(v.begin(), v.end(), 5), v.end());
		// теперь в v = {0};
	}
	{
		std::vector<int> v = { 0 };
		v.erase(remove_nth(v.begin(), v.end(), 0), v.end());
		// теперь в v = {0};
	}
	{
		std::vector<int> v = {};
		v.erase(remove_nth(v.begin(), v.end(), 0), v.end());
		// теперь в v = {};
	}
}

template<class Iterator, class Pred>
Iterator remove_if_(Iterator p, Iterator q, Pred pred)
{
	Iterator s = find_if(p, q, pred);
	if (s == q)
		return q;

	Iterator out = s;
	++s;
	return remove_copy_if(s, q, out, pred);
}

void elementNi_test() {
	{
		std::cout << "ElementN1 test:\n";
		std::vector<int> v1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		std::vector<int> v2 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		auto e = ElementN1(3);
		v1.erase(remove_if(v1.begin(), v1.end(), e), v1.end()); for (int i : v1) std::cout << i << ' '; std::cout << std::endl;	// " "
		v2.erase(remove_if(v2.begin(), v2.end(), e), v2.end()); for (int i : v2) std::cout << i << ' '; std::cout << std::endl;	// " "
	}
	{
		std::cout << "ElementN2 test:\n";
		std::vector<int> v1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		std::vector<int> v2 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		auto e = ElementN2(3);
		v1.erase(remove_if(v1.begin(), v1.end(), e), v1.end()); for (int i : v1) std::cout << i << ' '; std::cout << std::endl;	// " "
		v2.erase(remove_if(v2.begin(), v2.end(), e), v2.end()); for (int i : v2) std::cout << i << ' '; std::cout << std::endl;	// " "
	}
	// runtime error
	//{ 
	//	std::cout << "ElementN3 test:\n";
	//	std::vector<int> v1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
	//	std::vector<int> v2 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
	//	auto e = ElementN3(3);
	//	v1.erase(remove_if(v1.begin(), v1.end(), e), v1.end()); for (int i : v1) std::cout << i << ' '; std::cout << std::endl;	// " "
	//	v2.erase(remove_if(v2.begin(), v2.end(), e), v2.end()); for (int i : v2) std::cout << i << ' '; std::cout << std::endl;	// " "
	//}
	{
		std::cout << "ElementN4 test:\n";
		std::vector<int> v1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		std::vector<int> v2 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		auto e = ElementN4(3);
		v1.erase(remove_if(v1.begin(), v1.end(), e), v1.end()); for (int i : v1) std::cout << i << ' '; std::cout << std::endl;	// " "
		v2.erase(remove_if(v2.begin(), v2.end(), e), v2.end()); for (int i : v2) std::cout << i << ' '; std::cout << std::endl;	// " "
	}
	// compile time error
	//{
	//	std::cout << "ElementN5 test:\n";
	//	std::vector<int> v1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
	//	std::vector<int> v2 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
	//	auto e = ElementN5(3);
	//	v1.erase(remove_if(v1.begin(), v1.end(), e), v1.end()); for (int i : v1) std::cout << i << ' '; std::cout << std::endl;	// " "
	//	v2.erase(remove_if(v2.begin(), v2.end(), e), v2.end()); for (int i : v2) std::cout << i << ' '; std::cout << std::endl;	// " "
	//}
	{
		std::cout << "ElementN6 test:\n";
		std::vector<int> v1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		std::vector<int> v2 = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		auto e = ElementN6(3);
		v1.erase(remove_if(v1.begin(), v1.end(), e), v1.end()); for (int i : v1) std::cout << i << ' '; std::cout << std::endl;	// " "
		v2.erase(remove_if(v2.begin(), v2.end(), e), v2.end()); for (int i : v2) std::cout << i << ' '; std::cout << std::endl;	// " "
	}
}

int main()
{
	listDemo();

	std::list<int> const l0 = { };
	size_t len0 = max_increasing_len(l0.begin(), l0.end());
	std::list<int> const l1 = { 7,8,9,4,5,6,1,2,3,4 };
	size_t len1 = max_increasing_len(l1.begin(), l1.end()); // 4 coresponding to "1,2,3,4"
	std::list<int> const l2 = { -3,-2,-1,0,0,1,2,3,4,5 };
	size_t len2 = max_increasing_len(l2.begin(), l2.end()); // 6 coresponding to "0,1,2,3,4,5"
	
	/*
	 * Multimethod2 task >>>>>>>
	 */
	// multimethod for the heirs of Shape
	// returning bool and commutative
	Multimethod2<Shape, bool, true> is_intersect;
	// add multimethod implementation for two rectangles
	is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);
	// add multimethod implementation for a rectangle and a triangle
	is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);
	// create two shapes
	Shape * t1 = new Triangle();
	Shape * r1 = new Rectangle();

	std::cout << "s1: " << typeid(*t1).name() << std::endl; // Triangle
	std::cout << "s2: " << typeid(*r1).name() << std::endl; // Rectangle

	auto it1 = is_intersect.getImpl(t1, r1);
	auto it2 = is_intersect.getImpl(r1, t1);

	// check that implementation for s1 and s2 exists
	if (is_intersect.hasImpl(t1, r1))
	{
		// is_intersect_r_t(s2, s1) gets called
		bool res = is_intersect.call(t1, r1);
		     res = is_intersect.call(r1, t1);
		// Note: is_intersect_r_t receives
		// a rectangle as the first arg and a triangle as the second
		// here the shapes are passed conversely;
		// implementation must handle this case properly
	}
	/*
	 * Multimethod2 task <<<<<<<
	 */
	
	advance1<decltype(is_intersect.getImpl(t1,r1))>(it1, 1);

	reverseIterator_demo();

	// read sequence of double's from input.txt to v
	std::ifstream file("input.txt");
	/* Important note!
	 * ===============
	 * if the outer brackets would be ommited in the expression "(std::istream_iterator<double>(file))",
	 * the whole expression below will be not the 'v' variable definition with type of vector,
	 * but the function declaration named 'v' which recieves two parms:
	 *  o parm1 - istream_iterator<double>
	 *  o parm2 - anonymous pointer to a function which receives no parms and returns istream_iterator<double>
	 * and returns vector<double>.
	 * The clue: 
	 *  - declaration of a func parms can't be surrounded with brackets, but an arg (in a func call) could be"
	 *    - this fact allows to let the compiler know what we mean
	 *
	 * More info:
	 *  o http://easy-coding.blogspot.com/2009/02/c.html?m=1
	 */
	std::vector<double> v((std::istream_iterator<double>(file)), std::istream_iterator<double>());
	// Taking the trick described above, it's better to code it as below:
	auto f_it1 = std::istream_iterator<double>(file);
	auto f_it2 = std::istream_iterator<double>();
	std::vector<double> v_(f_it1, f_it2);

	rangeClass_demo();

	vectorList_test();

	algorithms_demo();

	remove_nth_test();

	{
		std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
		v.erase(remove_if_(v.begin(), v.end(), ElementN1(3)), v.end());
		for (int i : v)				// "0 1 2 4 5 6 8 9 10 11 12 "
			std::cout << i << ' ';
		std::cout << std::endl;
	}

	elementNi_test();
}