#pragma once
#include <string>

//This templated type will make a type const if "is_const" is true
template<typename T, bool is_const>
using make_const_if_true = typename std::conditional<is_const, const T, T>::type;

namespace sorting_impl
{
	//The default comparer. Returns true if "a" is less than "b" and false otherwise
	template<typename DataType>
	constexpr int DefaultComparer(const DataType& a, const DataType& b)
	{
		return a < b;
	}
}


std::string version();