#pragma once

#include <vector>

template <class T> size_t sizeof_vec_data(const std::vector<T>& v) {
	return sizeof(T) * v.size();
}
