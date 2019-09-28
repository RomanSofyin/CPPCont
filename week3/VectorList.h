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
		for (const VectT& v : data_) {
			n += v.size();
		}
		return n;
	}

	// TBD: const_iterator
	using VectorList_const_iterator = std::iterator<
		std::bidirectional_iterator_tag,				// category
		const T,										// value_type
		size_t,											// defference_type
		const T *,										// pointer
		const T &>;										// reference
	
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

		const_iterator& operator=(const const_iterator&) = default;
		
		bool operator==(const const_iterator& rhs) const {
			bool b = (d == rhs.d) && (it_l == rhs.it_l) && (it_v == rhs.it_v);
			return b;
		}
		bool operator!=(const const_iterator& rhs) const {
			return !operator==(rhs);
		}

		typename VectorList_const_iterator::reference operator*() const {
			// I don't know if I need to allow this for vlist having NULL vector (result of VectT::const_iterator())
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
		
		// Операции, необходимые для InputIterator.
		typename VectorList_const_iterator::pointer operator->() const {
			return &(*it_v);	// the same as "return &it_v.operator*()"
		}

		// Операции, необходимые для BidirectionalIterator.
		const_iterator& operator--() {
			dec();
			return *this;
		}
		const_iterator operator--(int) {
			auto old(*this);		// copy old value
			this->operator--();		// pre-increment
			return old;				// return old value
		}

					const ListT*			d;		// poiter to an instance of list<vector<T>> (aka ListT);					can't be NULL because VectorList() initializes VectorList::data_ with list<>();
		typename	ListT::const_iterator	it_l;	// *iterator* to an element of list<vector<T>>	which type is vector<T>;	can't be NULL because d can't be NULL; in case of empty *d, it is d->end() ( <==> d->begin() )
		typename	VectT::const_iterator	it_v;	// *iterator* to an element of vector<T>		which type is T;			can   be NULL when *d is empty
	private:
		void inc() {
			/* General note for iterators:
			 * ===========================
			 * operators ++, --, *, and -> are dangerous
			 * it should be checked that the element, pointed to by the iterator, exists
			 */
			if (it_l == d->end())			// if VectorList is an empty list
				return;						//     we have nothing to do
			// VectorList is not an empty list
			if (++it_v != it_l->end())
				return;
			if (++it_l != d->end()) {
				it_v = it_l->begin();
				return;
			}
			--it_l;
			it_v = it_l->end();
		}

		void dec() {
			if (it_l == d->end())			// if VectorList is an empty list
				return;						//     we have nothing to do
			if (it_v != it_l->begin()) {
				it_v--;
				return;
			}
			if (it_l != d->begin()) {
				it_l--;
				it_v = --(it_l->end());
				return;
			}
			return;
		}
	};

	const_iterator begin() const {
		typename VectT::const_iterator const_it_v;					// aka std::_Vector_const_iterator, pointing to a T; *default constructor gets called here - VectT::const_iterator()
		typename ListT::const_iterator const_it_l = data_.begin();	// aka std::_List_const_iterator, pointing to a vector
		if (const_it_l != data_.end())								// if data_ is not empty list; data_ itself won't be null because it gets initialized in VectorList()
			const_it_v = const_it_l->begin();
		return const_iterator(&this->data_, const_it_l, const_it_v);
	}

	const_iterator end() const {
		typename VectT::const_iterator const_it_v;
		typename ListT::const_iterator const_it_l = data_.end();
		if (const_it_l != data_.begin()) {							// if data_ is not empty list
			const_it_v = (--const_it_l)->end();
		}
		return const_iterator(&this->data_, const_it_l, const_it_v);
	}

	struct const_reverse_iterator : public VectorList_const_iterator {
		const_reverse_iterator() = default;
		const_reverse_iterator(const const_reverse_iterator&) = default;
		const_reverse_iterator(
			const ListT* d,
			typename ListT::const_reverse_iterator rit_l,
			typename VectT::const_reverse_iterator rit_v
		) : d(d), rit_l(rit_l), rit_v(rit_v)
		{}
		const_reverse_iterator(const_iterator & it) {
			d = it.d;
			rit_l = ListT::const_reverse_iterator(it.it_l);
			rit_v = VectT::const_reverse_iterator(it.it_v);
		}
		const_iterator base() const {
			return const_iterator(d,
				rit_l.base(),
				rit_v.base()
			);
		}
		~const_reverse_iterator() = default;

		const_reverse_iterator& operator=(const const_reverse_iterator&) = default;
		
		bool operator==(const const_reverse_iterator& rhs) const {
			bool b = (d == rhs.d) && (rit_l == rhs.rit_l) && (rit_v == rhs.rit_v);
			return b;
		}
		bool operator!=(const const_reverse_iterator& rhs) const {
			return !operator==(rhs);
		}

		typename VectorList_const_iterator::reference operator*() const {
			return *rit_v;
		}

		const_reverse_iterator& operator++() {
			rinc();
			return *this;
		}
		const_reverse_iterator operator++(int) {
			auto old(*this);		// copy old value
			this->operator++();		// pre-increment
			return old;				// return old value
		}

		typename VectorList_const_iterator::pointer operator->() const {
			return &(*rit_v);	// the same as "return &it_v.operator*()"
		}

		const_reverse_iterator& operator--() {
			rdec();
			return *this;
		}
		const_reverse_iterator operator--(int) {
			auto old(*this);		// copy old value
			this->operator--();		// pre-increment
			return old;				// return old value
		}
	private:
		const ListT* d;
		typename ListT::const_reverse_iterator rit_l;
		typename VectT::const_reverse_iterator rit_v;

		void rinc() {
			if (rit_l == d->rend())			// if VectorList is an empty list
				return;						//     we have nothing to do
			// VectorList is not an empty list
			if (++rit_v != rit_l->rend())
				return;
			if (++rit_l != d->rend()) {
				rit_v = rit_l->rbegin();
				return;
			}
			--rit_l;
			rit_v = rit_l->rend();
		}

		void rdec() {
			if (rit_l == d->rend())			// if VectorList is an empty list
				return;						//     we have nothing to do
			// VectorList is not an empty list
			if (rit_v != rit_l->rbegin()) {
				rit_v--;
				return;
			}
			if (rit_l != d->rbegin()) {
				rit_l--;
				rit_v = --(rit_l->rend());
				return;
			}
			return;
		}
	};

	const_reverse_iterator rbegin() const {
		typename VectT::const_reverse_iterator const_rit_v;
		typename ListT::const_reverse_iterator const_rit_l = data_.rbegin();
		if (const_rit_l != data_.rend()) {
			const_rit_v = const_rit_l->rbegin();
		}
		return const_reverse_iterator(&data_, const_rit_l, const_rit_v);
	}

	const_reverse_iterator rend() const {
		typename VectT::const_reverse_iterator const_rit_v;
		typename ListT::const_reverse_iterator const_rit_l = data_.rend();
		if (const_rit_l != data_.rbegin()) {
			const_rit_v = (--const_rit_l)->rend();
		}
		return const_reverse_iterator(&data_, const_rit_l, const_rit_v);
	}

private:
	ListT data_;
};

