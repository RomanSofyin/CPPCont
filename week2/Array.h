#pragma once

template<class T>
struct Array
{
	// все объ€вленные ниже методы (7 штук) уже реализованы
	// *RIS: taken from CPPIntro week6
	explicit Array(size_t size = 0) : size_(size) {
		data_ = (T*) new char[size * sizeof(T)];	// obtain sufficient amount of storage
		// init all the obj's with new default values
		for (size_t i = 0; i < size_; i++)
			new (data_ + i) T();
	}
	// *RIS: taken from CPPIntro week6
	Array(Array const& a) : size_(a.size_) {
		data_ = (T*) new char[size_ * sizeof(T)];
		for (size_t i = 0; i < size_; i++)
			new (data_ + i) T(a.data_[i]);			// placement new usage
	}
	// *RIS: taken from CPPIntro week6
	Array & operator=(Array const& a) {
		if (this != &a)
			Array<T>(oArr).swap(*this);
		return *this;
	}
	// *RIS: taken from CPPIntro week6
	~Array() {
		for (size_t i = 0; i < size_; i++)
			data_[i].~T();
		delete[](char *)data_;
		size_ = 0;
	}
	// *RIS: taken from CPPIntro week6
	size_t size() const { return size_; }
	// *RIS: taken from CPPIntro week6
	T & operator[](size_t i) { return *(data_ + i); }
	// *RIS: taken from CPPIntro week6
	T const& operator[](size_t i) const { return *(data_ + i); }

	// реализуйте перемещающий  конструктор
	//
	// реализуйте перемещающий оператор присваивани€

private:
	size_t  size_;
	T *     data_;

	void swap(Array & a) {
		std::swap(size_, a.size_);
		std::swap(data_, a.data);
	}
};