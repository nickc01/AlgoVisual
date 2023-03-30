#pragma once
#include <vector>
#include <string>
#include <memory>

enum class Option
{
	LinkedList,
	BinarySearchTree,
	Graph,
	BubbleSort,
	InsertionSort,
	MergeSort,
	QuickSort,
	HeapSort,

	MAX
};

std::vector<std::string> create_option_strings();

std::unique_ptr<const char*[]> convertToCharArrays(const std::vector<std::string>& values);