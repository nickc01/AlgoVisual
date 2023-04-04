#include <options.h>
#include <type_traits>

/*std::vector<std::string> create_option_strings() {
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
		case Option::QuickSort:
			optionNames.push_back("Quick Sort");
			break;
		default:
			optionNames.push_back("UNKNOWN OPTION");
			break;
		}
	}
	return optionNames;
}*/