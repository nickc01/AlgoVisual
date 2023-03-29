#include <gtest/gtest.h>
#include <common_traits.h>
#include <linked_list.h>

TEST(LinkedListTests, CopyConstructorTest)
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

	//Try to make a copy
	linked_list<int> copy = linked_list<int>(testList);
	//Test if the copy and the original are equal to each other
	ASSERT_TRUE(std::equal(copy.begin(), copy.end(), testList.begin(), testList.end()));
}

TEST(LinkedListTests, MoveConstructorTest)
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

	//Store the original size
	int originalSize = testList.getSize();


	//Move the list to the new variable
	linked_list<int> movedList = linked_list<int>(std::move(testList));

	//Test of the size is retained and the old list's size is 0
	ASSERT_TRUE(movedList.getSize() == originalSize && testList.getSize() == 0);
}

TEST(LinkedListTests, CopyAssignmentTest)
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

	//Assign the testList to the copy to copy it
	linked_list<int> copy = testList;

	//Test if they are both equal
	ASSERT_TRUE(std::equal(copy.begin(), copy.end(), testList.begin(), testList.end()));
}

TEST(LinkedListTests, MoveAssignmentTest)
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

	//Store the original size
	int originalSize = testList.getSize();


	//Assign the list to the movedList via std::move
	linked_list<int> movedList = std::move(testList);

	//Test if the movedList has the same size and the original list's size is 0
	ASSERT_TRUE(movedList.getSize() == originalSize && testList.getSize() == 0);
}

TEST(LinkedListTests, FindTest)
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

	//Find the iterator for the number, which should be the iterator after the begin() iterator
	auto foundNum = testList.find(2819);

	//Test if the found iterator is after the begin() iterator
	ASSERT_TRUE(foundNum == ++testList.begin());
}

TEST(LinkedListTests, ConstFindTest)
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

	//Assign the list to a const reference
	const auto& constList = testList;

	//Find the iterator for the number, which should be the iterator after the begin() iterator
	auto foundNum = constList.find(2819);

	//Test if the found iterator is after the begin() iterator
	ASSERT_TRUE(foundNum == ++constList.begin());
}

TEST(LinkedListTests, IteratorTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);

	//Calculate the sum
	int sum = 6523 + 2819 + 9302 + 43829;

	//Stores the list's actual sum
	int iteratorSum = 0;

	//Loop over all the elements in the list
	for (auto& num : testList)
	{
		//Add each of the numbers to the sum
		iteratorSum += num;
	}

	//Test if the sums are equal
	ASSERT_EQ(iteratorSum, sum);
}

TEST(LinkedListTests, ConstIteratorTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);

	//Assign the list to a const reference
	const auto& constList = testList;

	//Calculate the sum
	int sum = 6523 + 2819 + 9302 + 43829;

	//Stores the list's actual sum
	int iteratorSum = 0;

	//Loop over all the elements in the list
	for (auto& num : constList)
	{
		//Add each of the numbers to the sum
		iteratorSum += num;
	}

	//Test if the sums are equal
	ASSERT_EQ(iteratorSum, sum);
}

TEST(LinkedListTests, PushBackTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(2819);
	testList.push_back(9302);
	testList.push_back(43829);

	//Test if the last VALID element is equal to 43829
	ASSERT_TRUE(43829 == *(--testList.end()));
}

TEST(LinkedListTests, PushFrontTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_front(6523);
	testList.push_front(43829);
	testList.push_front(2819);
	testList.push_front(9302);

	//Test if the first element is equal to 9302
	ASSERT_TRUE(9302 == *(testList.begin()));
}

TEST(LinkedListTests, InsertCopyTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(2819);
	testList.push_back(9302);

	//Get the iterator for a number
	auto iterBefore = testList.find(43829);

	//Insert the number 1234 before that
	auto newIter = testList.insert(1234, iterBefore);

	//Test if the number 1234 has been inserted before 43829
	ASSERT_TRUE(newIter == (--iterBefore));
}

TEST(LinkedListTests, InsertMoveTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(43829);
	testList.push_back(6523);
	testList.push_back(9302);

	//Find the number 6523
	auto iterBefore = testList.find(6523);

	//The new number to be inserted
	int value = 4321;

	//Insert the value by moving it
	auto newIter = testList.insert(std::move(value), iterBefore);

	//Test if the number 4321 is before the number 6523
	ASSERT_TRUE(newIter == (--iterBefore));
}

TEST(LinkedListTests, DeleteElementTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	//Get the size before the deletion
	int size = testList.getSize();

	//Find a number in the list
	auto iter = testList.find(6523);

	//Delete the found number
	testList.pop_element(iter);

	//Find the element that was right after the one we deleted
	auto afterIter = testList.find(43829);

	//Test if the element has been deleted and the size decreased
	ASSERT_TRUE(6523 != *(--afterIter) && (size - 1) == testList.getSize());

	//Assert::AreNotEqual(6523, *(--afterIter));
}

TEST(LinkedListTests, ClearTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	//Clear the list
	testList.clear();

	//Test if the size is 0
	ASSERT_EQ(testList.getSize(), 0);
}

TEST(LinkedListTests, SizeTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	//Test if the size is now 4
	ASSERT_EQ(testList.getSize(), 4);
}

TEST(LinkedListTests, MaximumTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	
	//Test if the largest value found is 43829
	ASSERT_TRUE((*maximum(testList)) == 43829);
}

TEST(LinkedListTests, MinimumTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	//Test if the smallest value found is 2819
	ASSERT_TRUE((*minimum(testList)) == 2819);
}

TEST(LinkedListTests, MaximumConstTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	//Get a const reference to the list
	const auto& constList = testList;

	//Test if the largest value found is 43829
	ASSERT_TRUE((*maximum(testList)) == 43829);
}

TEST(LinkedListTests, MinimumConstTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	//Get a const reference to the list
	const auto& constList = testList;

	//Test if the smallest value found is 2819
	ASSERT_TRUE((*minimum(testList)) == 2819);
}

TEST(LinkedListTests, PrintTest)
{
	//Create a starting list
	linked_list<int> testList{};
	testList.push_back(2819);
	testList.push_back(6523);
	testList.push_back(43829);
	testList.push_back(9302);

	//Create a string stream
	std::stringstream stream;

	//Print the list to the stream
	stream << testList;

	//Convert the stream into a string
	auto result = stream.str();

	//Test if the string is the same as the expected output
	ASSERT_TRUE(result == std::string("[2819, 6523, 43829, 9302]"));
}