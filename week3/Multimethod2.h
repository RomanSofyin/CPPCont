#pragma once

// Base - base class of the hierarchy
// Result - type of the value returned by the multimethod
// Commutative - flag which indicates that the multimethod is commutative (i.e. f(x,y) = f(y,x)).
template<class Base, class Result, bool Commutative>
struct Multimethod2
{
private:
	typedef std::pair<std::type_index, std::type_index> mmap_key;
	typedef std::function <Result(Base*, Base*)>        mmap_fun;

	std::map<mmap_key, mmap_fun> mmap;

public:
	// sets implementation of the multimethod for the types t1 and t2 defined by typeid 
	// f - function (or functor) accepting two pointers to Base and returning a value with type of Result
	void addImpl(
		const type_info & t1,					// return type of
		const type_info & t2,					// . . typeid operator
		std::function<Result(Base*, Base*)> f)
	{
		// add an element to map so that the Multimethod2 instance will be commutative if needed
		mmap.emplace(
			std::make_pair(
				std::type_index(t1),
				std::type_index(t2)),
			f);
		if (Commutative)
			mmap.emplace(
				std::make_pair(
					std::type_index(t2),
					std::type_index(t1)),
				f);
	}

#define getImpl_RET_TYPE                               \
	std::_Tree_const_iterator<                         \
		std::_Tree_val<                                \
			std::_Tree_simple_types<                   \
				std::pair<                             \
					std::pair<                         \
						std::type_index,               \
						std::type_index                \
					> const,                           \
					std::function<                     \
						bool __cdecl(Shape*, Shape*)   \
					>                                  \
				>                                      \
			>                                          \
		>                                              \
	>

	auto getImpl(Base* a, Base* b) const
	{
		return mmap.find(
			std::make_pair(
				std::type_index(typeid(*a)),
				std::type_index(typeid(*b))));
	}
	
	// checks wheather implementation of the multimethod exists for the types of objects a and b
	bool hasImpl(Base* a, Base* b) const
	{
		// return true if implementation exists
		// if opeartion is commutative, presense of implementation b and a should be checked
		if (getImpl(a, b) != mmap.end())
			return true;
		return false;
	}

	// Applies multimethod to the objects via the pointers a and b
	Result call(Base* a, Base* b) const
	{
		// check Commutative
		if (dynamic_cast<Triangle*>(a) && dynamic_cast<Rectangle*>(b))
			std::swap(a, b);
		return getImpl(a, b)->second(a,b);
	}
};