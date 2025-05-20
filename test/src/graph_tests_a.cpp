#include <gtest/gtest.h>
#include <graph.h>
#include <string>

// Test the addition and removal of nodes
TEST(GraphTest, AddAndDeleteNodes) {
  graph<int> g;
  auto node1 = g.add_node(1);
  auto node2 = g.add_node(2);
  ASSERT_EQ(g.size(), 2);
  g.delete_node(node1);
  ASSERT_EQ(g.size(), 1);
  g.delete_node(node2);
  ASSERT_EQ(g.size(), 0);
}

// Test the addition and removal of connections
TEST(GraphTest, AddAndRemoveConnections) {
  graph<int> g;
  auto node1 = g.add_node(1);
  auto node2 = g.add_node(2);
  auto node3 = g.add_node(3);
  node1->add_connection_to(*node2);
  node1->add_connection_to(*node3);
  ASSERT_TRUE(node1->has_connection_to(*node2));
  ASSERT_TRUE(node1->has_connection_to(*node3));
  node1->remove_connection_to(*node2);
  ASSERT_FALSE(node1->has_connection_to(*node2));
  ASSERT_TRUE(node1->has_connection_to(*node3));
}

// Test the removal of a node and its connections
TEST(GraphTest, DeleteNodeAndConnections) {
  graph<int> g;
  auto node1 = g.add_node(1);
  auto node2 = g.add_node(2);
  auto node3 = g.add_node(3);
  node1->add_connection_to(*node2);
  node2->add_connection_to(*node3);
  g.delete_node(node2);
  ASSERT_EQ(g.size(), 2);
  ASSERT_FALSE(node1->has_connection_to(*node2));
  ASSERT_FALSE(node3->has_connection_to(*node2));
}

// Test finding nodes by value
TEST(GraphTest, FindNodeByValue) {
  graph<int> g;
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);
  auto node2 = g.find_node(2);
  ASSERT_NE(node2, g.end());
  ASSERT_EQ(node2->value, 2);
}

// Test finding nodes by a custom predicate
TEST(GraphTest, FindNodeByPredicate) {
  graph<int> g;
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);
  auto node2 = g.find_node_by([](const int &value) { return value == 2; });
  ASSERT_NE(node2, g.end());
  ASSERT_EQ(node2->value, 2);
}

// Test the graph equality operator
TEST(GraphTest, GraphEquality) {
  graph<int> g1;
  graph<int> g2;
  g1.add_node(1);
  g1.add_node(2);
  g2.add_node(1);
  g2.add_node(2);
  ASSERT_EQ(g1, g2);
}

// Test the graph inequality operator
TEST(GraphTest, GraphInequality) {
  graph<int> g1;
  graph<int> g2;
  g1.add_node(1);
  g1.add_node(2);
  g2.add_node(1);
  g2.add_node(3);
  ASSERT_NE(g1, g2);
}

// Test graph copy constructor and assignment operator
TEST(GraphTest, CopyAndMove) {
  graph<int> g1;
  g1.add_node(1);
  g1.add_node(2);

  graph<int> g2(g1);
  ASSERT_EQ(g1, g2);

  graph<int> g3;
  g3 = g1;
  ASSERT_EQ(g1, g3);

  graph<int> g4;
  g4 = std::move(g1);
  ASSERT_EQ(g4, g2);
}

// Test graph iterators
TEST(GraphTest, Iterators) {
  graph<int> g;
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);

  int sum = 0;
  for (auto it = g.begin(); it != g.end(); ++it) {
    sum += it->value;
  }
  ASSERT_EQ(sum, 6);
}

// Test graph clearing
TEST(GraphTest, Clear) {
  graph<int> g;
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);
  g.clear();
  ASSERT_EQ(g.size(), 0);
}
