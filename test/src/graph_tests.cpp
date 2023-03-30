#include <gtest/gtest.h>
#include <common.h>
#include <graph.h>
#include <gtest/gtest.h>

//Creates a testing graph
graph<int> CreateTestGraph()
{
	graph<int> testGraph;
	//Create a starting node
	auto firstNode = testGraph.add_node(0);

	//Create a second node
	auto secondNode = testGraph.add_node(5);

	//Create a one-way connection from the first node to the second node with a weight of 1

	firstNode->add_connection(secondNode, 1);

	//Add three new nodes that are one-way connected to the second node
	secondNode->add_connection(100, 2);
	secondNode->add_connection(200, 3);
	secondNode->add_connection(300, 4);

	//Return the test graph
	return testGraph;
}

TEST(GraphTests, CopyConstructorTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Create a copy of the graph
	graph<int> copy{ testGraph };

	//Test if the graphs are equal
	ASSERT_TRUE(testGraph == copy);
}

TEST(GraphTests, MoveConstructorTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Create a new graph by moving the existing graph
	graph<int> move{ std::move(testGraph) };

	//Test if the list has been moved
	ASSERT_TRUE(testGraph != move && testGraph.size() == 0);
}

TEST(GraphTests, CopyAssignmentTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Create a copy of the graph
	graph<int> copy = testGraph;

	//Test if the graphs are equal
	ASSERT_TRUE(testGraph == copy);
}

TEST(GraphTests, MoveAssignmentTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Create a new graph by moving the existing graph
	graph<int> move = std::move(testGraph);

	//Test if the list has been moved
	ASSERT_TRUE(testGraph != move && testGraph.size() == 0);
}

TEST(GraphTests, AddNodeTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Add a new node with the number 234
	testGraph.add_node(234);

	//See if the node "234" has been added
	ASSERT_TRUE(testGraph.find_node(234) != nullptr);
}

TEST(GraphTests, DeleteNodeTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Delete the node with the value "200" from the list
	testGraph.delete_node(200);

	//Test if the node has been removed
	ASSERT_TRUE(testGraph.find_node(200) == nullptr);
}

TEST(GraphTests, IteratorTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Store the amount of nodes in the graph
	std::list<graph<int>::node*> storageList;


	//Loop over all the nodes in the graph
	for (auto i = testGraph.begin(); i != testGraph.end(); i++)
	{
		//Add each node to the storage list
		storageList.push_back(i.operator->());
	}

	//Test if the storage list size and the test graph size is equal
	ASSERT_TRUE(storageList.size() == testGraph.size());
}

TEST(GraphTests, ConstIteratorTest)
{
	//Create a test graph
	const graph<int> testGraph = CreateTestGraph();

	//Store the amount of nodes in the graph
	std::list<const graph<int>::node*> storageList;

	//Loop over all the nodes in the graph
	for (auto i = testGraph.cbegin(); i != testGraph.cend(); i++)
	{
		//Add each node to the storage list
		storageList.push_back(i.operator->());
	}

	//Test if the storage list size and the test graph size is equal
	ASSERT_TRUE(storageList.size() == testGraph.size());
}

TEST(GraphTests, FindTest)
{
	//Create a test graph
	const graph<int> testGraph = CreateTestGraph();

	//Test if the node "200" exists in the graph
	ASSERT_TRUE(testGraph.find_node(200) != nullptr);
}

TEST(GraphTests, ClearTest)
{
	//Create a test graph
	graph<int> testGraph = CreateTestGraph();

	//Clear the graph
	testGraph.clear();

	//Check if the nodes list has no nodes
	ASSERT_TRUE(testGraph.size() == 0);
}

TEST(GraphTests, SizeTest)
{
	//Create a test graph
	const graph<int> testGraph = CreateTestGraph();

	//Test if there are 5 nodes in the graph
	ASSERT_TRUE(testGraph.size() == 5);
}

TEST(GraphTests, EqualityTest)
{
	//Create a test graph
	const graph<int> a = CreateTestGraph();
	//Create a second test graph
	const graph<int> b = CreateTestGraph();

	//Test if they are equal
	ASSERT_TRUE(a == b);
}

TEST(GraphTests, NonEqualityTest)
{
	//Create a test graph
	const graph<int> a = CreateTestGraph();
	//Create an empty graph
	const graph<int> b;

	//Test if they are non-equal
	ASSERT_TRUE(a != b);
}

TEST(GraphTests, PrintTest)
{
	//The stream to print to
	std::stringstream stream;

	//Create a test graph
	const graph<int> a = CreateTestGraph();

	//Print to the stream
	stream << a;

	//Convert the stream to the a string
	auto str = stream.str();

	//Check if the string is equal to the expected output
	ASSERT_TRUE(str == "Node {0}: Connections -> {5}\nNode {5}: Connections -> {100, 200, 300}\nNode {100}: Connections -> {}\nNode {200}: Connections -> {}\nNode {300}: Connections -> {}\n");
}