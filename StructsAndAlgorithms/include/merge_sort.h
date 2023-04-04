#pragma once
#include <type_traits>
#include <common.h>
#include <vector>
#include <functional>
#include <queue>


//This namespace contains implementation details
namespace merge_sort_impl
{
	//Finds the midpoint of an iteratorType range
	template<typename iteratorType>
	iteratorType get_midpoint(iteratorType begin, iteratorType end)
	{
		//Start the midpoint at the beginning
		iteratorType midPoint = begin;
		//Loop until the beginning is equal to the end
		while (begin != end)
		{
			//Increment the beginning iteratorType
			++begin;
			//If it is still not equal to the ened
			if (begin != end)
			{
				//Advance the midpoint one every time the begin iteratorType is incremented twice
				++begin;
				++midPoint;
			}
		}
		//Return the midpoint
		return midPoint;
	}

	//Splits an iteratorType range into two ranges. 
	//"aBegin", "aEnd", "bBegin", and "bEnd" are out parameters that will be updated when the function completes
	template<typename iteratorType>
	void split(iteratorType begin, iteratorType end, iteratorType& aBegin, iteratorType& aEnd, iteratorType& bBegin, iteratorType& bEnd)
	{
		//Find the midpoint
		iteratorType midPoint = get_midpoint(begin, end);

		//Split the ranges
		aBegin = begin;
		aEnd = midPoint;
		bBegin = midPoint;
		bEnd = end;
	}

	//Merges two iteratorType ranges into one. This uses an out-of-place sorting method
	template<typename iteratorType, typename Comparer>
	void merge_ranges(iteratorType aBegin, iteratorType aEnd, iteratorType bBegin, iteratorType bEnd, Comparer& comparer)
	{
		//If both ranges are empty, then nothing can be done
		if (aBegin == aEnd && bBegin == bEnd)
		{
			return;
		}

		//The queues for storing the contents of range A and range B out of place
		std::queue<typename std::remove_reference<decltype(*aBegin)>::type> listA;
		std::queue<typename std::remove_reference<decltype(*aBegin)>::type> listB;
		

		//Add the elements of range a to listA
		for (auto i = aBegin; i != aEnd; i++)
		{
			listA.emplace(std::move(*i));
		}

		//Add the elements of range b to listB
		for (auto i = bBegin; i != bEnd; i++)
		{
			listB.emplace(std::move(*i));
		}

		//Loop over the combined total amount of elements
		for (auto i = aBegin; i != bEnd; i++)
		{
			//If listA is currently empty, then pull from listB
			if (listA.size() == 0)
			{
				//Move the value from list B to the iteratorType
				(*i) = std::move(listB.front());
				//Remove the latest element from listB
				listB.pop();
			}
			//If listB is empty, then pull from listA
			else if (listB.size() == 0)
			{
				//Move the value from list A to the iteratorType
				(*i) = std::move(listA.front());
				//Remove the latest element from listA
				listA.pop();
			}
			//If the first value in listA is less than the first value of listB, then pull from listA
			else if (comparer(listA.front(), listB.front()))
			{
				//Move the value from list A to the iteratorType
				(*i) = std::move(listA.front());
				//Remove the latest element from listA
				listA.pop();
			}
			//If the first value in listA is greater than or equal to the first value of listB, then pull from listB
			else
			{
				//Move the value from list B to the iteratorType
				(*i) = std::move(listB.front());
				//Remove the latest element from listB
				listB.pop();
			}
		}
	}
}

//Runs a merge sort algorithm on the iteratorType range with the specified comparer
template<typename iteratorType, typename Comparer, typename Swapper = decltype(sorting_impl::DefaultSwapper<iteratorType>)>
void merge_sort(iteratorType&& begin, iteratorType&& end, Comparer& comparer, Swapper swapper = sorting_impl::DefaultSwapper<iteratorType>)
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

	//Split the list into two parts, "a" and "b"
	ValueType aBegin = end;
	ValueType aEnd = end;
	ValueType bBegin = end;
	ValueType bEnd = end;

	//Split the list into two parts and update "aBegin", "aEnd", "bBegin", and "bEnd" accordingly
	merge_sort_impl::split(begin, end, aBegin, aEnd, bBegin, bEnd);

	//Merge sort range A
	merge_sort(aBegin, aEnd, comparer);
	//Merge sort range B
	merge_sort(bBegin, bEnd, comparer);

	//Merge the two ranges into one
	merge_sort_impl::merge_ranges(aBegin, aEnd, bBegin, bEnd, comparer);
}

//Runs a merge sort algorithm on the iteratorType range
template<typename iteratorType>
void merge_sort(iteratorType&& begin, iteratorType&& end)
{
	merge_sort(std::forward<iteratorType>(begin), std::forward<iteratorType>(end), sorting_impl::DefaultComparer<decltype(*begin)>);
}

//Runs a merge sort algorithm on the list
template<typename iteratable>
void merge_sort(iteratable& list)
{
	merge_sort(std::begin(list), std::end(list), sorting_impl::DefaultComparer<decltype(*std::begin(list))>);
}