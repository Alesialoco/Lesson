#include <iostream>
#include "DynamicArr.h"

const double DynamicArr::coeff_ = 2;

DynamicArr::DynamicArr() {
	arr_ = new double[capacity_];
}

DynamicArr::DynamicArr(size_t size, double elem) : size_(size)
	,capacity_(coeff_* size_)
	,arr_(new double[capacity_]) {
	for (size_t i = 0; i < size_; ++i) {
		arr_[i] = elem;
	}
}

DynamicArr::DynamicArr(const DynamicArr& rhs) {
	size_ = rhs.size_;
	capacity_ = rhs.capacity_;
	arr_ = new double[capacity_];
	for (size_t i = 0; i < size_; ++i) {
		arr_[i] = rhs.arr_[i];
	}
}

DynamicArr::~DynamicArr() {
	delete[] arr_;
}

size_t DynamicArr::size() const {
	return size_;
}

size_t DynamicArr::capacity() const {
	return capacity_;
}

void DynamicArr::push_back(double elem) {
	if (size_ == capacity_) {
		capacity_ = coeff_ * capacity_;
		double* tmp = new double[capacity_];
		for (size_t i = 0; i < size_; ++i) {
			tmp[i] = arr_[i];
		}
		delete[] arr_;
		arr_ = tmp;
	}
	arr_[size_++] = elem;
}

DynamicArr& DynamicArr::operator=(DynamicArr other){
    capacity_ = coeff_ * other.capacity_;
	double* tmp = new double[capacity_];
	for (size_t i = 0; i < other.size_; ++i) {
		tmp[i] = other.arr_[i];
	}
	delete[] arr_;
	arr_ = tmp;
    return *this;
}

double DynamicArr::operator[](const int index){
	if (index <= size_){
		return arr_[index];
	}
}
