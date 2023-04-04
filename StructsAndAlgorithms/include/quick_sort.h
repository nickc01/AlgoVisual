#pragma once

#include <type_traits>
#include <common.h>
#include <functional>
#include <vector>

//Runs a insertion sort algorithm on the iterator range with the specified comparer
template<typename iteratorType, typename Comparer, typename Swapper = decltype(sorting_impl::DefaultSwapper<iteratorType>)>
void quick_sort(iteratorType&& begin, iteratorType&& end, Comparer& comparer, Swapper swapper = sorting_impl::DefaultSwapper<iteratorType>)
{
	//Removes the reference from the type to make it a value type
	using ValueType = typename std::remove_reference<iteratorType>::type;
	//If the size of the list is 0, no sorting is necessary
	if (begin == end)
	{
		return;
	}
	else
	{
		//Stores the next iteratorType
		ValueType next = begin;
		//If the size of the list is 1, no sorting is necessary
		if (++next == end)
		{
			return;
		}
	}

	//The pivot that will be used to compare values. 
	//Lower values to to the left of the pivot, and higher values to the right
	ValueType pivot = end;
	--pivot;

	//An iterator to keep track of where the lower values end
	ValueType lowEnd = begin;

	for (auto i = begin; i != end; i++)
	{
		//If the current index value is lower than the pivot, it should go to the left
		if (comparer(*i,*pivot))
		{
			//Move the value to the left side of the array
			swapper(lowEnd,i);
			//Increment the lower end iterator
			++lowEnd;
		}
	}

	//Move the pivot to where the lower values end.
	//At this point, all lower values should be left of the pivot, while greater values to the right of the pivot
	swapper(lowEnd, pivot);

	//Quick sort the lower values
	quick_sort(begin, lowEnd, comparer);
	
	//Quick sort the higher values
	quick_sort(lowEnd, end, comparer);
}

//Runs a insertion sort algorithm on the iterator range
template<typename iteratorType>
void quick_sort(iteratorType&& begin, iteratorType&& end)
{
	quick_sort(std::forward<iteratorType>(begin), std::forward<iteratorType>(end), sorting_impl::DefaultComparer<decltype(*begin)>);
}

//Runs a insertion sort algorithm on the list
template<typename iteratable>
void quick_sort(iteratable& list)
{
	quick_sort(std::move(list.begin()), std::move(list.end()), sorting_impl::DefaultComparer<decltype(*std::begin(list))>);
}