#include <gtest/gtest.h>
#include <graph.h>

// Test fixture for graph_node
template <typename T>
class GraphNodeTest : public ::testing::Test {
protected:
    using Node = graph_node<T>;
    T value1;
    T value2;
    T value3;

    void SetUp() override {
        if constexpr (std::is_same_v<T, std::string>) {
            value1 = "node1";
            value2 = "node2";
            value3 = "node3";
        } else {
            value1 = 1;
            value2 = 2;
            value3 = 3;
        }
    }
};

// Test fixture for graph
template <typename T>
class GraphTest : public ::testing::Test {
protected:
    using Graph = graph<T>;
    using Node = graph_node<T>;
    T value1;
    T value2;

    void SetUp() override {
        if constexpr (std::is_same_v<T, std::string>) {
            value1 = "node1";
            value2 = "node2";
        } else {
            value1 = 42;
            value2 = 43;
        }
    }
};

// Test cases for graph_node
TYPED_TEST_SUITE_P(GraphNodeTest);

TYPED_TEST_P(GraphNodeTest, ValueConstructor) {
    // Test the constructor that initializes the node with a value.
    typename TestFixture::Node node(this->value1);
    EXPECT_EQ(node.value, this->value1);
    EXPECT_EQ(node.connections_size(), 0);
}

TYPED_TEST_P(GraphNodeTest, AddConnection) {
    // Test adding a connection between two nodes.
    typename TestFixture::Node node1(this->value1);
    typename TestFixture::Node node2(this->value2);
    EXPECT_TRUE(node1.add_connection_to(node2));
    EXPECT_EQ(node1.connections_size(), 1);
    EXPECT_TRUE(node1.has_connection_to(node2));
}

TYPED_TEST_P(GraphNodeTest, RemoveConnection) {
    // Test removing a connection between two nodes.
    typename TestFixture::Node node1(this->value1);
    typename TestFixture::Node node2(this->value2);
    node1.add_connection_to(node2);
    EXPECT_TRUE(node1.remove_connection_to(node2));
    EXPECT_EQ(node1.connections_size(), 0);
    EXPECT_FALSE(node1.has_connection_to(node2));
}

TYPED_TEST_P(GraphNodeTest, RemoveAllConnections) {
    // Test removing all connections from a node.
    typename TestFixture::Node node1(this->value1);
    typename TestFixture::Node node2(this->value2);
    typename TestFixture::Node node3(this->value3);
    node1.add_connection_to(node2);
    node1.add_connection_to(node3);
    node1.remove_all_connections();
    EXPECT_EQ(node1.connections_size(), 0);
}

REGISTER_TYPED_TEST_SUITE_P(GraphNodeTest, ValueConstructor, AddConnection, RemoveConnection, RemoveAllConnections);

// Test cases for graph
TYPED_TEST_SUITE_P(GraphTest);

TYPED_TEST_P(GraphTest, DefaultConstructor) {
    // Test the default constructor of the graph.
    typename TestFixture::Graph graph;
    EXPECT_EQ(graph.size(), 0);
}

TYPED_TEST_P(GraphTest, AddNode) {
    // Test adding a node to the graph.
    typename TestFixture::Graph graph;
    auto node = graph.add_node(this->value1);
    EXPECT_EQ(graph.size(), 1);
    EXPECT_EQ(node->value, this->value1);
}

TYPED_TEST_P(GraphTest, DeleteNode) {
    // Test deleting a node from the graph.
    typename TestFixture::Graph graph;
    auto node = graph.add_node(this->value1);
    graph.delete_node(node);
    EXPECT_EQ(graph.size(), 0);
}

TYPED_TEST_P(GraphTest, DeleteNodeByValue) {
    // Test deleting a node by its value.
    typename TestFixture::Graph graph;
    graph.add_node(this->value1);
    graph.delete_node_by_value(this->value1);
    EXPECT_EQ(graph.size(), 0);
}

TYPED_TEST_P(GraphTest, FindNode) {
    // Test finding a node by its value.
    typename TestFixture::Graph graph;
    auto node = graph.add_node(this->value1);
    auto found_node = graph.find_node(this->value1);
    EXPECT_EQ(*found_node, *node);
}

TYPED_TEST_P(GraphTest, ClearGraph) {
    // Test clearing all nodes from the graph.
    typename TestFixture::Graph graph;
    graph.add_node(this->value1);
    graph.add_node(this->value2);
    graph.clear();
    EXPECT_EQ(graph.size(), 0);
}

REGISTER_TYPED_TEST_SUITE_P(GraphTest, DefaultConstructor, AddNode, DeleteNode, DeleteNodeByValue, FindNode, ClearGraph);

// Instantiate the tests for different types
using NodeTypes = ::testing::Types<int, std::string>;
INSTANTIATE_TYPED_TEST_SUITE_P(My, GraphNodeTest, NodeTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(My, GraphTest, NodeTypes);

