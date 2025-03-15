#pragma once

template <class T>
constexpr const T& clamp(const T& val, const T& low, const T& high) {
	return (val < low) ? low : (high < val) ? high : val;
}
