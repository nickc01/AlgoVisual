#pragma once
#include <type_traits>
#include <common.h>
#include <vector>
#include <functional>

//Runs a insertion sort algorithm on the iterator range with the specified comparer
template<typename iteratorType, typename Comparer, typename Swapper = std::function<decltype(sorting_impl::DefaultSwapper<iteratorType>)>>
void insertion_sort(iteratorType&& begin, iteratorType&& end, Comparer&& comparer, Swapper swapper = sorting_impl::DefaultSwapper<iteratorType>)
{
	//Loop from the beginning to the end
	for (iteratorType i = begin; i != end; i++)
	{
		//Get the current index
		iteratorType current = i;
		//Loop until the current moves back to the beginning
		while (current != begin)
		{
			//Get the previous element
			iteratorType previous = current;
			--previous;
			//If "current" is less than "previous"
			if (comparer(*current, *previous))
			{
				//Swap the current element with the previous element
				swapper(current, previous);
			}
			//If "current" is greater than or equal to "previous"
			else
			{
				//No need to swap any more and the loop can be broken to move on to the next elements
				break;
			}
			//Move to the previous element
			--current;
		}
	}
}

//Runs a insertion sort algorithm on the iterator range
template<typename iteratorType>
void insertion_sort(iteratorType&& begin, iteratorType&& end)
{
	insertion_sort(std::forward<iteratorType>(begin), std::forward<iteratorType>(end), sorting_impl::DefaultComparer<decltype(*begin)>);
}

//Runs a insertion sort algorithm on the list
template<typename iteratable>
void insertion_sort(iteratable& list)
{
	insertion_sort(std::begin(list), std::end(list), sorting_impl::DefaultComparer<decltype(*std::begin(list))>);
}