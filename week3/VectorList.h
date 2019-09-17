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
	template<class It> void append(It p, It q) {
		if (p != q)
			data_.push_back(VectT(p, q));
	} // ??? it is defined outside of class ???

	bool empty() const { return size() == 0; }

	size_t size() const
	{
		size_t n = 0;
		for (VectT v : data_) {
			n += v.size();
		}
		return n;
	}

	// TBD: const_iterator
	struct const_iterator :	public std::iterator<
			std::bidirectional_iterator_tag,	// category
			const T,							// value_type
			size_t,								// defference_type
			const T *,							// pointer
			const T &> {						// reference
		//const_iterator() = default;
		//const_iterator(const const_iterator&) = default;
		//const_iterator& operator=(const const_iterator&) = default;
		const_iterator(const T * e, typename VectT::iterator it_v, typename ListT::iterator it_l)
		: elem(e), it_vect(it_v), it_list(it_l) {}
		~const_iterator() = default;
		/*reference operator*() const { return .dereference(); }
		iterator& operator++() { pos.increment(); return *this; }
		iterator operator++(int) { auto old = *this; ++(*this); return old; }

		// Операции, необходимые для InputIterator.
		pointer operator->() const;

		// Операции, необходимые для BidirectionalIterator.
		iterator& operator--() { pos.decrement(); return *this; }
		iterator operator--(int) { auto old = *this; --(*this); return old; }*/
	private:
		const T* elem;
		typename VectT::iterator it_vect;
		typename ListT::iterator it_list;
	};

	// TBD: begin - end
	const_iterator begin() const {
		auto a = *(data_.begin()->begin());
		auto b = data_.begin();
		auto c = data_.begin()->begin();
		auto d = const_iterator(a, c, b);
		return const_iterator();
		/*return const_iterator(
			*(data_.begin()->begin()),
			data_.begin(),
			data_.begin()->begin()
		);*/
	}
	//const_iterator end()   const { return ...; }

	// TBD: const_reverse_iterator
	/*... const_reverse_iterator ...*/

	// TBD: rbegin - rend
	/*const_reverse_iterator rbegin() const { return ...; }*/
	/*const_reverse_iterator rend()   const { return ...; }*/

private:
	ListT data_;
};

