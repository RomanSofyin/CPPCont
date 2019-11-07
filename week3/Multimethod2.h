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
		const std::type_info & t1,					// return type of
		const std::type_info & t2,					// . . typeid operator
		std::function<Result(Base*, Base*)> f)
	{
		// add an element to map so that the Multimethod2 instance will be commutative if needed
		mmap.emplace(
			std::make_pair(
				std::type_index(t1),
				std::type_index(t2)),
			f);
		/*
		if (Commutative)
			mmap.emplace(
				std::make_pair(
					std::type_index(t2),
					std::type_index(t1)),
				f);
				*/
	}

// This is the type the 'it' obj in call() func assigned to
#define getImpl_RET_TYPE1                              \
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
#define getImpl_RET_TYPE2 mmap.find(std::make_pair(std::type_index(typeid(*a)),std::type_index(typeid(*b))))
	// getImpl() is commutative if needed
	// Commented out the line below some kind of which should probably be used to complete the task on stepik
	//auto getImpl(Base* a, Base* b, bool c = Commutative) -> decltype(getImpl_RET_TYPE2) const
	// may resume investigation of the issue with getImpl() from here - https://stackoverflow.com/questions/5973427/error-passing-xxx-as-this-argument-of-xxx-discards-qualifiers
    auto getImpl(Base* a, Base* b, bool c = Commutative) const -> decltype(getImpl_RET_TYPE2)
	{
		auto it = mmap.find(
			std::make_pair(
				std::type_index(typeid(*a)),
				std::type_index(typeid(*b))
			)
		);
		if (it == mmap.end() && c)
			it = mmap.find(std::make_pair(std::type_index(typeid(*b)), std::type_index(typeid(*a))));
		return it;
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
	// If call() gets called then it is guaranteed that the implementation exsists for a and b considering Commutative
	Result call(Base* a, Base* b) const
	{
		auto it = getImpl(a, b, false);
		if (it != mmap.end())
			return it->second(a, b);
		return getImpl(a, b)->second(b, a);
	}
};