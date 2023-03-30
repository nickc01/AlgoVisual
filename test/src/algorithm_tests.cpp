#include <gtest/gtest.h>
#include <common.h>
#include <insertion_sort.h>
#include <merge_sort.h>
#include <linked_list.h>
#include <bubble_sort.h>
#include <quick_sort.h>
#include <heap_sort.h>

TEST(InsertionAlgorithm, Sort)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);
	testList.push_back(9201);
	testList.push_back(3920);
	testList.push_back(932);

	//The same elements, but sorted
	linked_list<int> sortedList{};
	sortedList.push_back(932);
	sortedList.push_back(2819);
	sortedList.push_back(3920);
	sortedList.push_back(6523);
	sortedList.push_back(9201);
	sortedList.push_back(9302);
	sortedList.push_back(43829);

	//Sort the list
	insertion_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(InsertionAlgorithm, SortEmpty)
{
	//Create a starting list
	linked_list<int> testList{};

	//The same elements, but sorted
	linked_list<int> sortedList{};

	//Sort the list
	insertion_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(MergeAlgorithm, Sort)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);
	testList.push_back(9201);
	testList.push_back(3920);
	testList.push_back(932);

	//The same elements, but sorted
	linked_list<int> sortedList{};
	sortedList.push_back(932);
	sortedList.push_back(2819);
	sortedList.push_back(3920);
	sortedList.push_back(6523);
	sortedList.push_back(9201);
	sortedList.push_back(9302);
	sortedList.push_back(43829);

	//Sort the list
	merge_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(MergeAlgorithm, SortEmpty)
{
	//Create a starting list
	linked_list<int> testList{};

	//The same elements, but sorted
	linked_list<int> sortedList{};

	//Sort the list
	merge_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(BubbleAlgorithm, Sort)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);
	testList.push_back(9201);
	testList.push_back(3920);
	testList.push_back(932);

	//The same elements, but sorted
	linked_list<int> sortedList{};
	sortedList.push_back(932);
	sortedList.push_back(2819);
	sortedList.push_back(3920);
	sortedList.push_back(6523);
	sortedList.push_back(9201);
	sortedList.push_back(9302);
	sortedList.push_back(43829);

	//Sort the list
	bubble_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(Algorithm, SortEmpty)
{
	//Create a starting list
	linked_list<int> testList{};

	//The same elements, but sorted
	linked_list<int> sortedList{};

	//Sort the list
	bubble_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(QuickSortAlgorithm, Sort)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);
	testList.push_back(9201);
	testList.push_back(3920);
	testList.push_back(932);

	//The same elements, but sorted
	linked_list<int> sortedList{};
	sortedList.push_back(932);
	sortedList.push_back(2819);
	sortedList.push_back(3920);
	sortedList.push_back(6523);
	sortedList.push_back(9201);
	sortedList.push_back(9302);
	sortedList.push_back(43829);

	//Sort the list
	quick_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(QuickSortAlgorithm, SortEmpty)
{
	//Create a starting list
	linked_list<int> testList{};

	//The same elements, but sorted
	linked_list<int> sortedList{};

	//Sort the list
	quick_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(HeapSortAlgorithm, Sort)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);
	testList.push_back(9201);
	testList.push_back(3920);
	testList.push_back(932);

	//The same elements, but sorted
	linked_list<int> sortedList{};
	sortedList.push_back(932);
	sortedList.push_back(2819);
	sortedList.push_back(3920);
	sortedList.push_back(6523);
	sortedList.push_back(9201);
	sortedList.push_back(9302);
	sortedList.push_back(43829);

	//Sort the list
	heap_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}

TEST(HeapSortAlgorithm, SortEmpty)
{
	//Create a starting list
	linked_list<int> testList{};

	//The same elements, but sorted
	linked_list<int> sortedList{};

	//Sort the list
	heap_sort(testList);

	//Test if the list has been properly sorted
	ASSERT_EQ(testList, sortedList);
}