#pragma once

/* This is just a try to get a clue of interators and how to implement one */

template<long FROM, long TO>
class Range {
public:
	// member typedefs provided through inheriting from std::iterator
	class mySuperPuperIterator : public std::iterator<
		std::input_iterator_tag,   // iterator_category
		long,                      // value_type
		long,                      // difference_type
		const long*,               // pointer
		long                       // reference
	> {
		long num = FROM;
	public:
		explicit mySuperPuperIterator(long _num = 0) : num(_num) {}
		mySuperPuperIterator& operator++() { num = TO >= FROM ? num + 1 : num - 1; return *this; }
		mySuperPuperIterator operator++(int) { mySuperPuperIterator retval = *this; ++(*this); return retval; }
		bool operator==(mySuperPuperIterator other) const { return num == other.num; }
		bool operator!=(mySuperPuperIterator other) const { return !(*this == other); }
		reference operator*() const { return num; }
	};
	mySuperPuperIterator begin() { return mySuperPuperIterator(FROM); }
	mySuperPuperIterator end() { return mySuperPuperIterator(TO >= FROM ? TO + 1 : TO - 1); }
};