#pragma once

#include <type_traits>
#include <common.h>

//Runs a insertion sort algorithm on the list
template<typename iteratable>
void bubble_sort(iteratable& list)
{
	bubble_sort(std::begin(list), std::end(list), sorting_impl::DefaultComparer<decltype(*std::begin(list))>);
}

//Runs a insertion sort algorithm on the iterator range
template<typename iteratorType>
void bubble_sort(iteratorType&& begin, iteratorType&& end)
{
	bubble_sort(std::forward<iteratorType>(begin), std::forward<iteratorType>(end), sorting_impl::DefaultComparer<decltype(*begin)>);
}

//Runs a insertion sort algorithm on the iterator range with the specified comparer
template<typename iteratorType, typename Comparer>
void bubble_sort(iteratorType&& begin, iteratorType&& end, Comparer& comparer)
{
	while (true)
	{
		bool sorted = true;

		//Loop from the beginning to the end
		for (iteratorType i = begin; i != end; i++)
		{
			//Get the next element
			auto next = i;
			next++;

			//if the next element is beyond the end of the list, then stop
			if (next == end)
			{
				break;
			}

			//if the next element is less than the current
			if (comparer(*next, *i))
			{
				//Swap the next and current elements
				std::swap(*i, *next);

				//Mark the list as unsorted
				sorted = false;
			}
		}

		//if the list is sorted
		if (sorted)
		{
			//Break out of the loop
			return;
		}
	}
}