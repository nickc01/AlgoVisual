#pragma once

#include <type_traits>
#include <common.h>
#include <binary_search_tree.h>
#include <functional>
#include <vector>

//Runs a insertion sort algorithm on the list
template<typename iteratable>
void heap_sort(iteratable& list)
{
	heap_sort(std::begin(list), std::end(list), sorting_impl::DefaultComparer<decltype(*std::begin(list))>);
}

//Runs a insertion sort algorithm on the iterator range
template<typename iteratorType>
void heap_sort(iteratorType&& begin, iteratorType&& end)
{
	heap_sort(std::forward<iteratorType>(begin), std::forward<iteratorType>(end), sorting_impl::DefaultComparer<decltype(*begin)>);
}

//Runs a insertion sort algorithm on the iterator range with the specified comparer
template<typename iteratorType, typename Comparer>
void heap_sort(iteratorType&& begin, iteratorType&& end, Comparer& comparer)
{
	using valueType = typename std::remove_reference<decltype(*begin)>::type;

	binary_search_tree<valueType> tree {};

	//Add all the values into the tree
	//When all the values are inserted, we can use the tree's iterator to iterate from lowest to highest value
	for (auto i = begin; i != end; i++)
	{
		tree.insert(std::move(*i));
	}

	auto k = begin;

	for (auto i = tree.begin(); i != tree.end(); i++)
	{
		*k = std::move(*i);
		++k;
	}
}