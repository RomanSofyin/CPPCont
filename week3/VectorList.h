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
	using VectorList_const_iterator = std::iterator<
		std::bidirectional_iterator_tag,		// category
		const T,								// value_type
		size_t,									// defference_type
		const T *,								// pointer
		const T &>;								// reference
	struct const_iterator :	public VectorList_const_iterator {
		
		const_iterator() = default;
		const_iterator(const const_iterator&) = default;
		const_iterator(
			const ListT* d,
			typename ListT::const_iterator it_l,
			typename VectT::const_iterator it_v
		) : d(d), it_l(it_l), it_v(it_v)
		{}
		~const_iterator() = default;

		//const_iterator& operator=(const const_iterator&) = default;
		
		bool operator==(const const_iterator& rhs) {
			return true;
		}
		bool operator!=(const const_iterator& rhs) {
			return !operator==(rhs);
		}

		typename VectorList_const_iterator::reference operator*()  {
			return *it_v;
		}

		const_iterator& operator++() {
			inc();
			return *this;
		}
		const_iterator operator++(int) {
			auto old(*this);		// copy old value
			this->operator++();		// pre-increment
			return old;				// return old value
		}
		/*
		// Операции, необходимые для InputIterator.
		pointer operator->() const;

		// Операции, необходимые для BidirectionalIterator.
		iterator& operator--() { pos.decrement(); return *this; }
		iterator operator--(int) { auto old = *this; --(*this); return old; }*/
	private:
		const ListT * d;
		typename VectT::const_iterator it_v;
		typename ListT::const_iterator it_l;

		void inc() {
			//auto b = it_v;
		}
	};

	// TBD: begin - end
	const_iterator begin() const {
		auto const_it_l = data_.begin();		// ListT::const_iterator (aka std::_List_const_iterator)   pointing to a vector
		auto const_it_v = const_it_l->begin();	// VectT::const_iterator (aka std::_Vector_const_iterator) pointing to a T
		return const_iterator(&this->data_, const_it_l, const_it_v);
	}

	const_iterator end() const {
		auto const_it_l = data_.end();          // ListT::const_iterator (aka std::_List_const_iterator)   pointing to a vector
		auto const_it_v = const_it_l->end();	// VectT::const_iterator (aka std::_Vector_const_iterator) pointing to a T
		return const_iterator(&this->data_, const_it_l, const_it_v);
	}

	// TBD: const_reverse_iterator
	/*... const_reverse_iterator ...*/

	// TBD: rbegin - rend
	/*const_reverse_iterator rbegin() const { return ...; }*/
	/*const_reverse_iterator rend()   const { return ...; }*/

private:
	ListT data_;
};

