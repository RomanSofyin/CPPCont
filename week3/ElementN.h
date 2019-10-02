#pragma once

// 1
struct ElementN1
{
	explicit ElementN1(size_t n)
		: n(n), i(0)
	{}

	template<class T>
	bool operator()(T const& t) { return (i++ == n); }

	size_t n;
	size_t i;
};

//2 
struct ElementN2
{
	explicit ElementN2(size_t n)
		: n(n)
	{}

	template<class T>
	bool operator()(T const& t) { return (i++ == n); }

	size_t n;
	static size_t i;
};
size_t ElementN2::i = 0;

// 3 
struct ElementN3
{
	explicit ElementN3(size_t n)
		: n(n), pi(new size_t(0))
	{}

	template<class T>
	bool operator()(T const& t) { return ((*pi)++ == n); }

	~ElementN3() { delete pi; }

	size_t n;
	size_t * pi;
};

// 4
struct ElementN4
{
	explicit ElementN4(size_t n)
		: n(n), pi(new size_t(0))
	{}

	template<class T>
	bool operator()(T const& t) { return ((*pi)++ == n); }

	size_t n;
	std::shared_ptr<size_t> pi;
};

// 5
struct ElementN5
{
	explicit ElementN5(size_t n)
		: n(n), pi(new size_t(0))
	{}

	template<class T>
	bool operator()(T const& t) { return ((*pi)++ == n); }

	size_t n;
	std::unique_ptr<size_t> pi;
};

// 6 
struct ElementN6
{
	explicit ElementN6(size_t n)
		: n(n)
	{}

	template<class T>
	bool operator()(T const& t)
	{
		static size_t i = 0;
		return (i++ == n);
	}

	size_t n;
};