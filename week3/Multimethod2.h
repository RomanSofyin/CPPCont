#pragma once

// Base - base case of the hierarchy
// Result - type of the value returned by the multimethod
// Commutative - flag which indicates that the multimethod is commutative (i.e. f(x,y) = f(y,x)).
template<class Base, class Result, bool Commutative>
struct Multimethod2
{
	// sets implementation of the multimethod
	// for the types t1 and t2 defined by typeid 
	// f - function (or functor)
	// accepting two pointers to Base 
	// and returning a value with type of Result
	void addImpl(const type_info & t1, const type_info & t2, std::function<Result (Base*, Base*)> f)
	{
		
	}

	// checks wheather implementation of the multimethod exists for the types of objects a and b
	bool hasImpl(Base* a, Base* b) const
	{
		// return true if implementation exists
		// if opeartion is commutative, presense of implementation b and a should be checked
		return ...;
	}

	// Applies multimethod to the objects via the pointers a and b
	Result call(Base* a, Base* b) const
	{
		// returns the result of applying multimethod to a and b
		return ...;
	}

private:
	std::map<std::pair<std::type_index, std::type_index>, std::function> mmap;
};