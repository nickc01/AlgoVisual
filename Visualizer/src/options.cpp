#include <options.h>
#include <type_traits>

namespace
{
	//std::unique_ptr<std::string[]> optionNames;
	//std::unique_ptr<const char*[]> optionNamesCStr;
}

std::vector<std::string> create_option_strings() {
	std::vector<std::string> optionNames{};

	for (auto i = 0; i < (int)Option::MAX; i++)
	{
		auto value = (Option)i;
		switch (value)
		{
		case Option::LinkedList:
			optionNames.push_back("Linked List");
			break;
		case Option::BinarySearchTree:
			optionNames.push_back("Binary Search Tree");
			break;
		case Option::Graph:
			optionNames.push_back("Graph");
			break;
		case Option::BubbleSort:
			optionNames.push_back("Bubble Sort");
			break;
		case Option::InsertionSort:
			optionNames.push_back("Insertion Sort");
			break;
		case Option::MergeSort:
			optionNames.push_back("Merge Sort");
			break;
		case Option::QuickSort:
			optionNames.push_back("Quick Sort");
			break;
		case Option::HeapSort:
			optionNames.push_back("Heap Sort");
			break;
		default:
			optionNames.push_back("UNKNOWN OPTION");
			break;
		}
	}
	return optionNames;
}

std::unique_ptr<const char*[]> convertToCharArrays(const std::vector<std::string>& values)
{
	auto ptr = std::unique_ptr<const char* []>(new const char*[values.size()]);

	for (int i = 0; i < values.size(); i++)
	{
		ptr[i] = values[i].c_str();
	}
	return ptr;
}