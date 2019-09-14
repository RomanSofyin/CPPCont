#pragma once

template<class T>
class VectorList
{
private:
	using VectT  = std::vector<T>;
	using ListT = std::list<VectT>;

public:
	using value_type = T;

	VectorList() = default;
	VectorList(VectorList const&) = default;
	VectorList(VectorList&&) = default;

	VectorList& operator=(VectorList&&) = default;
	VectorList& operator=(VectorList const&) = default;

	// Method which will be used to fill a VectorList
	// It guarantees that there will not be empty arrays
	template<class It>
	void append(It p, It q); // it defined outside of class
/*  {
		if (p != q)
			data_.push_back(VectT(p,q));
	}
*/

	bool empty() const { return size() == 0; }

	// TBD: size
	size_t size() const
	{
		...
	}

	// TBD: const_iterator
	... const_iterator ...

		// TBD: begin / end
	const_iterator begin() const { return ...; }
	const_iterator end()   const { return ...; }

	// TBD: const_reverse_iterator
	... const_reverse_iterator ...

		// TBD: rbegin / rend
	const_reverse_iterator rbegin() const { return ...; }
	const_reverse_iterator rend()   const { return ...; }

private:
	ListT data_;
};