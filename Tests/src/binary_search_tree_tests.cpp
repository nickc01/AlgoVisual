#include <gtest/gtest.h>
#include <common_traits.h>
#include "binary_search_tree.h"
#include <gtest/gtest.h>

TEST(BinarySearchTree, Insert)
{
    binary_search_tree<int> bst;

    ASSERT_TRUE(bst.insert(5) != bst.end());
    ASSERT_TRUE(bst.insert(3) != bst.end());
    ASSERT_TRUE(bst.insert(7) != bst.end());
    ASSERT_TRUE(bst.insert(2) != bst.end());
    ASSERT_TRUE(bst.insert(4) != bst.end());
    ASSERT_TRUE(bst.insert(6) != bst.end());
    ASSERT_TRUE(bst.insert(8) != bst.end());
    ASSERT_TRUE(bst.insert(5) == bst.end());
    ASSERT_EQ(bst.getSize(), 7);

    bst.clear();
    ASSERT_EQ(bst.getSize(), 0);
}

TEST(BinarySearchTree, Find)
{
    binary_search_tree<int> bst;

    ASSERT_TRUE(bst.insert(5) != bst.end());
    ASSERT_TRUE(bst.insert(3) != bst.end());
    ASSERT_TRUE(bst.insert(7) != bst.end());
    ASSERT_TRUE(bst.insert(2) != bst.end());
    ASSERT_TRUE(bst.insert(4) != bst.end());
    ASSERT_TRUE(bst.insert(6) != bst.end());
    ASSERT_TRUE(bst.insert(8) != bst.end());

    ASSERT_TRUE(bst.find(5) != bst.end());
    ASSERT_TRUE(bst.find(3) != bst.end());
    ASSERT_TRUE(bst.find(7) != bst.end());
    ASSERT_TRUE(bst.find(2) != bst.end());
    ASSERT_TRUE(bst.find(4) != bst.end());
    ASSERT_TRUE(bst.find(6) != bst.end());
    ASSERT_TRUE(bst.find(8) != bst.end());
    ASSERT_TRUE(bst.find(9) == bst.end());

    bst.clear();
    ASSERT_TRUE(bst.find(5) == bst.end());
}

TEST(BinarySearchTree, Remove)
{
    binary_search_tree<int> bst;

    ASSERT_TRUE(bst.insert(5) != bst.end());
    ASSERT_TRUE(bst.insert(3) != bst.end());
    ASSERT_TRUE(bst.insert(7) != bst.end());
    ASSERT_TRUE(bst.insert(2) != bst.end());
    ASSERT_TRUE(bst.insert(4) != bst.end());
    ASSERT_TRUE(bst.insert(6) != bst.end());
    ASSERT_TRUE(bst.insert(8) != bst.end());

    ASSERT_TRUE(bst.remove(bst.find(5)));
    ASSERT_TRUE(bst.find(5) == bst.end());
    ASSERT_EQ(bst.getSize(), 6);

    bst.clear();
    ASSERT_EQ(bst.getSize(), 0);
}

TEST(BinarySearchTree, CopyConstructorTest)
{
	//Create a starting list
	binary_search_tree<int> testList{};
	testList.insert(6523);
	testList.insert(2819);
	testList.insert(9302);
	testList.insert(43829);
	testList.insert(9201);
	testList.insert(3920);
	testList.insert(932);

	//Try to make a copy
	binary_search_tree<int> copy = binary_search_tree<int>(testList);
	//Test if the copy and the original are equal to each other
	ASSERT_TRUE(std::equal(copy.begin(), copy.end(), testList.begin(), testList.end()));
}

TEST(BinarySearchTree, MoveConstructorTest)
{
	//Create a starting list
	binary_search_tree<int> testList{};
	testList.insert(6523);
	testList.insert(2819);
	testList.insert(9302);
	testList.insert(43829);
	testList.insert(9201);
	testList.insert(3920);
	testList.insert(932);

	//Store the original size
	int originalSize = testList.getSize();


	//Move the list to the new variable
	binary_search_tree<int> movedList = binary_search_tree<int>(std::move(testList));

	//Test of the size is retained and the old list's size is 0
	ASSERT_TRUE(movedList.getSize() == originalSize && testList.getSize() == 0);
}

TEST(BinarySearchTree, CopyAssignmentTest)
{
	//Create a starting list
	binary_search_tree<int> testList{};
	testList.insert(6523);
	testList.insert(2819);
	testList.insert(9302);
	testList.insert(43829);
	testList.insert(9201);
	testList.insert(3920);
	testList.insert(932);

	//Assign the testList to the copy to copy it
	binary_search_tree<int> copy = testList;

	//Test if they are both equal
	ASSERT_TRUE(std::equal(copy.begin(), copy.end(), testList.begin(), testList.end()));
}

TEST(BinarySearchTree, MoveAssignmentTest)
{
	//Create a starting list
	binary_search_tree<int> testList{};
	testList.insert(6523);
	testList.insert(2819);
	testList.insert(9302);
	testList.insert(43829);
	testList.insert(9201);
	testList.insert(3920);
	testList.insert(932);

	//Store the original size
	int originalSize = testList.getSize();


	//Assign the list to the movedList via std::move
	binary_search_tree<int> movedList = std::move(testList);

	//Test if the movedList has the same size and the original list's size is 0
	ASSERT_TRUE(movedList.getSize() == originalSize && testList.getSize() == 0);
}

TEST(BinarySearchTree, ClearTest)
{
	//Create a starting list
	binary_search_tree<int> testList{};
	testList.insert(6523);
	testList.insert(2819);
	testList.insert(9302);
	testList.insert(43829);
	testList.insert(9201);
	testList.insert(3920);
	testList.insert(932);

	auto oldSize = testList.getSize();

	testList.clear();

	ASSERT_EQ(testList.getSize(),0);
}

TEST(BinarySearchTree, MinimumTest)
{
	//Create a starting list
	binary_search_tree<int> testList{};
	testList.insert(6523);
	testList.insert(2819);
	testList.insert(9302);
	testList.insert(43829);
	testList.insert(9201);
	testList.insert(3920);
	testList.insert(932);

	ASSERT_EQ(*testList.minimum(), 932);
}

TEST(BinarySearchTree, MaximumTest)
{
	//Create a starting list
	binary_search_tree<int> testList{};
	testList.insert(6523);
	testList.insert(2819);
	testList.insert(9302);
	testList.insert(43829);
	testList.insert(9201);
	testList.insert(3920);
	testList.insert(932);

	ASSERT_EQ(*testList.maximum(), 43829);
}

TEST(BinarySearchTree, IteratorTest) {
	binary_search_tree<int> bst;
	bst.insert(10);
	bst.insert(5);
	bst.insert(15);
	bst.insert(3);
	bst.insert(7);
	bst.insert(12);
	bst.insert(18);

	std::vector<int> expected_values = { 3, 5, 7, 10, 12, 15, 18 };

	// Test forward iterator
	std::vector<int> forward_values;
	for (auto it = bst.begin(); it != bst.end(); ++it) {
		forward_values.push_back(*it);
	}
	ASSERT_EQ(forward_values, expected_values);

	// Test const iterator
	std::vector<int> const_values;
	for (auto it = bst.cbegin(); it != bst.cend(); ++it) {
		const_values.push_back(*it);
	}
	ASSERT_EQ(const_values, expected_values);
}

TEST(BinarySearchTree, GetRootTest) {
	//BALANCED TEST
	binary_search_tree<int> bst;
	EXPECT_EQ(bst.getRoot(), bst.end());

	bst.insert(50);
	bst.insert(30);
	bst.insert(20);
	bst.insert(40);
	bst.insert(10);

	EXPECT_NE(bst.getRoot(), bst.end());
	EXPECT_EQ(*bst.getRoot(), 30);

	bst.clear();
	EXPECT_EQ(bst.getSize(), 0);

	//UNBALANCED TEST
	bst.setSelfBalancing(false);
	EXPECT_EQ(bst.getSelfBalancing(),false);

	EXPECT_EQ(bst.getRoot(), bst.end());

	bst.insert(50);
	bst.insert(30);
	bst.insert(20);
	bst.insert(40);
	bst.insert(10);

	EXPECT_NE(bst.getRoot(), bst.end());
	EXPECT_EQ(*bst.getRoot(), 50);
}

TEST(BinarySearchTree, TraverseTest) {
	binary_search_tree<int> bst;
	bst.insert(50);
	bst.insert(30);
	bst.insert(20);
	bst.insert(40);
	bst.insert(70);
	bst.insert(60);
	bst.insert(80);

	std::vector<int> expected{ 20, 30, 40, 50, 60, 70, 80 };
	std::vector<int> actual = bst.traverse();

	EXPECT_EQ(actual, expected);
}

TEST(BinarySearchTree, PrintTest)
{
	//Create a starting list
	binary_search_tree<int> bst{};
	bst.insert(50);
	bst.insert(30);
	bst.insert(20);
	bst.insert(40);
	bst.insert(70);
	bst.insert(60);
	bst.insert(80);

	//Create a string stream
	std::stringstream stream;

	//Print the list to the stream
	stream << bst;

	//Convert the stream into a string
	auto result = stream.str();

	//Test if the string is the same as the expected output
	ASSERT_TRUE(result == std::string("[20, 30, 40, 50, 60, 70, 80]"));
}