#include "DynamicArr.h"

#include <iostream>

template <class T>
const double DynamicArr<T>::coeff_ = 2;

template <class T>
DynamicArr<T>::DynamicArr() {
	arr_ = new T[capacity_];
}

template <class T>
DynamicArr<T>::DynamicArr(size_t size, T elem) : size_(size)
, capacity_(coeff_* size_)
, arr_(new T[capacity_]) {
	for (size_t i = 0; i < size_; ++i) {
		arr_[i] = elem;
	}
}

template <class T>
DynamicArr<T>::DynamicArr(const DynamicArr<T>& rhs) {
	size_ = rhs.size_;
	capacity_ = rhs.capacity_;
	arr_ = new T[capacity_];
	for (size_t i = 0; i < size_; ++i) {
		arr_[i] = rhs.arr_[i];
	}
}

template <class T>
DynamicArr<T>::DynamicArr(DynamicArr<T>&& rhs) :size_(rhs.size_), arr_(rhs.arr_) { 
    rhs.arr = nullptr;
    rhs.size = 0;
}

template <class T>
DynamicArr<T>::~DynamicArr() {
	delete[] arr_;
}

template <class T>
size_t DynamicArr<T>::size() const {
	return size_;
}

template<class T> 
size_t DynamicArr<T>::capacity() const {
	return capacity_;
}

template <class T>
void DynamicArr<T>::push_back(T elem) {
	if (size_ == capacity_) {
		capacity_ = coeff_ * capacity_;
		T* tmp = new T[capacity_];
		for (size_t i = 0; i < size_; ++i) {
			tmp[i] = arr_[i];
		}
		delete[] arr_;
		arr_ = tmp;
	}
	arr_[size_++] = elem;
}

template <class T>
const T& DynamicArr<T>::operator[](size_t index) const {
	return arr_[index];
}

template <class T>
T& DynamicArr<T>::operator[](size_t index) {
	return arr_[index];
}

template <class T>
DynamicArr<T>& DynamicArr<T>::operator=(const DynamicArr<T>& rhs) {
	if (this == &rhs)
		return *this;
	delete[] arr_;
	arr_ = new T[rhs.capacity()];
	for (int i = 0; i < rhs.size(); ++i) {
		arr_[i] = rhs[i];
		std::cout << arr_[i] << " ";
	}
	return *this;
}

template <class T>
DynamicArr<T>& DynamicArr<T>::operator=(DynamicArr<T>&& rhs) { 
    arr_ = rhs.arr_;
    size_ = rhs.size_;
    rhs.arr_ = nullptr;
    rhs.size_ = 0;
    return *this;
}

void fun() {

	DynamicArr<double> arr1, arr2(3, 1);
	DynamicArr<double> arr3(arr1);

	for (size_t i = 0; i < 3; ++i) {
		arr3.push_back(static_cast<double>(i));
	}

	for (size_t i = 0; i < arr3.size(); ++i) {
		std::cout << arr3[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "arr3.capacity() = " << arr3.capacity() << std::endl;

	for (size_t i = 0; i < arr2.size(); ++i) {
		std::cout << arr2[i] << " ";
	}
}