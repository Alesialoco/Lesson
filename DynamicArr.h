#pragma once

class DynamicArr {
	static const size_t initialCapacity_ = 2;	// only one for all objects of DynamicArr
	static const double coeff_;					// same as above

	size_t size_ = 0;
	size_t capacity_ = initialCapacity_;
	double* arr_;

public:
	DynamicArr();
	DynamicArr(size_t size, double element = 0);
	DynamicArr(const DynamicArr& rhs);
	~DynamicArr();
	size_t size() const;
	size_t capacity() const;
	void push_back(double elem);
	DynamicArr& operator=(DynamicArr other);
	double operator[](const int index);
};