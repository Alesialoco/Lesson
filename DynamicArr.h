#pragma once
#include <stddef.h>

template <class T>
class DynamicArr {
	static const size_t initialCapacity_ = 2;	// only one for all objects of DynamicArr
	static const double coeff_;					// same as above

	size_t size_ = 0;
	size_t capacity_ = initialCapacity_;
	T* arr_;

public:
	DynamicArr();
	explicit DynamicArr(size_t size, T elem = 0);
	DynamicArr(const DynamicArr<T>& rhs);
	DynamicArr(DynamicArr<T>&& rhs);
	~DynamicArr();
	size_t size() const;
	size_t capacity() const;
	void push_back(T elem);
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	DynamicArr<T>& operator=(const DynamicArr& rhs);
	DynamicArr& operator=(DynamicArr<T>&& rhs);
};

void fun();