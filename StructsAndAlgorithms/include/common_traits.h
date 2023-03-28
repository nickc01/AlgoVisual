#pragma once
#include <string>

//This templated type will make a type const if "is_const" is true
template<typename T, bool is_const>
using make_const_if_true = typename std::conditional<is_const, const T, T>::type;


std::string version();

int multiplyTest(int a, int b);