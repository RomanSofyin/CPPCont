#pragma once

template<class T>
class VectorList
{
private:
	using VectT = std::vector<T>;
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

	size_t size() const
	{
		size_t n = 0;
		for (VectT v : data) {
			n += v.size();
		}
		return n;
	}

	// TBD: const_iterator
	struct std::bidirectional_iterator_tag const_iterator : 
		public std::interator< 	// public inheritance is used by default for structures (private - for classes)
			std::bidirectional_iterator_tag,	// category
			T,									// value_type
			size_t,								// defference_type
			const T *,							// pointer
			const T &> {						// reference
	private:
		T num;
	public:
		/*
		iterator() = default;
		iterator(const iterator&) = default;
		iterator& operator=(const iterator&) = default;
		~iterator() = default;
		reference operator*() const { return pos.dereference(); }
		iterator& operator++() { pos.increment(); return *this; }
		iterator operator++(int) { auto old = *this; ++(*this); return old; }

		// Операции, необходимые для InputIterator.
		pointer operator->() const;

		// Операции, необходимые для BidirectionalIterator.
		iterator& operator--() { pos.decrement(); return *this; }
		iterator operator--(int) { auto old = *this; --(*this); return old; }
		*/
	};

	// TBD: begin / end
	/*const_iterator begin() const { return ...; }*/
	/*const_iterator end()   const { return ...; }*/

	// TBD: const_reverse_iterator
	/*... const_reverse_iterator ...*/

	// TBD: rbegin / rend
	/*const_reverse_iterator rbegin() const { return ...; }*/
	/*const_reverse_iterator rend()   const { return ...; }*/

private:
	ListT data_;
};
// Просто чтобы проверить поддерживаются ли символы русского алфавита
