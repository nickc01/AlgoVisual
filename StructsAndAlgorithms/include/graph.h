#pragma once

#include "common.h"
#include <list>
#include <ostream>
#include <struct_exception.h>
#include <unordered_set>
#include <linked_list.h>

template<typename T>
class graph;

//Represents a node in a graph
template<typename T>
class graph_node {

	//Allows the graph class to see graph_node's internals
	friend class graph<T>;

	//The list of connections this node has
	linked_list<graph_node*> connections;

	using node_iter = typename linked_list<graph_node<T>>::iterator;
	using const_node_iter = typename linked_list<graph_node<T>>::const_iterator;
public:
	//The iterator used for iterating over the connections of a node
	template<bool is_const>
	class connection_iterator_base {
		make_const_if_true<linked_list<graph_node*>, is_const>* connections = nullptr;
		decltype(connections->begin()) iter;

	public:
		using value_type = make_const_if_true<graph_node, is_const>;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = int;

		connection_iterator_base() : connections(), iter() {}

		connection_iterator_base(decltype(connections) _connections, decltype(iter) _iter) : connections(_connections), iter(_iter) {}

		template<bool other_const>
		connection_iterator_base(const connection_iterator_base<other_const>& other) : connections(other.connections), iter(other.iter) {}

		template<bool other_const>
		connection_iterator_base(connection_iterator_base<other_const>&& other) noexcept : connections(std::move(other.connections)), iter(std::move(other.iter)) {}
		
		//Increments the iterator to the next element
		connection_iterator_base<is_const>& operator++()
		{
			++iter;
			return *this;
		}

		//Increments the iterator to the next element
		connection_iterator_base<is_const> operator++(int)
		{
			connection_iterator_base<is_const> previous = *this;

			this->operator++();

			return previous;
		}

		//Used for dereferencing the value
		const value_type& operator*() const
		{
			//Return a reference to the value
			return *(*iter);
		}

		//Used for dereferencing the value
		const pointer operator->() const
		{
			//Return a pointer to the value
			return *iter;
		}

		//Used for dereferencing the value
		value_type& operator*()
		{
			//Return a reference to the value
			return **iter;
		}

		//Used for dereferencing the value
		pointer operator->()
		{
			//Return a pointer to the value
			return *iter;
		}

		//Tests for inequality
		bool operator!=(const connection_iterator_base<is_const>& rhs) const
		{
			return connections != rhs.connections || iter != rhs.iter;
		}

		//Tests for equality
		bool operator==(const connection_iterator_base<is_const>& rhs) const
		{
			return connections == rhs.connections && iter == rhs.iter;
		}

		//A copy assignment operator used for assigning a non-const iterator to a const version
		template<bool other_const = is_const>
		connection_iterator_base<is_const>& operator=(const connection_iterator_base<other_const>& other) noexcept
		{
			connections = other.connections;
			iter = other.iter;
			return *this;
		}

		//A move assignment operator used for assigning a non-const iterator to a const version
		template<bool other_const = is_const>
		connection_iterator_base<is_const>& operator=(connection_iterator_base<other_const>&& other) noexcept
		{
			connections = std::move(other.connections);
			iter = std::move(other.iter);
			return *this;
		}
	};

	using iterator = connection_iterator_base<false>;
	using const_iterator = connection_iterator_base<true>;

	//The value of the node
	T value;

	graph_node() = delete;

	template <typename ValueType>
	graph_node(ValueType&& _value) : value(std::forward<ValueType>(_value)) {}

	//Adds a connection between this node and the provided destination node
	bool add_connection_to(graph_node<T>& node) {
		return connections.emplace_back(&node) != connections.end();
	}

	//Adds a connection between this node and the provided destination node
	bool add_connection_to(node_iter node) {
		return add_connection_to(*node);
	}

	//Removes a connection to a provided destination node
	bool remove_connection_to(const graph_node<T>& node) {
		auto result = connections.end();
		for (auto i = connections.begin(); i != connections.end(); i++) {
			if (*i == &node) {
				result = i;
			}
		}
		//auto result = connections.find(&graph_node);
		if (result != connections.end()) {
			connections.pop_element(result);
			return true;
		}
		return false;
	}

	//Removes a connection to a provided destination node
	bool remove_connection_to(node_iter node) {
		return remove_connection_to(*node);
	}

	//Removes all connections
	void remove_all_connections() {
		connections.clear();
	}

	//Returns true if this node is connected to the provided destination node
	bool has_connection_to(const graph_node<T>& node) const {
		for (auto i = connections.begin(); i != connections.end(); i++) {
			if (*i == &node) {
				return true;
			}
		}
		return false;
	}

	//Returns true if this node is connected to the provided destination node
	bool has_connection_to(node_iter node) {
		return has_connection_to(*node);
	}

	//Returns true if this node is connected to the provided destination node
	bool has_connection_to(const_node_iter node) const {
		return has_connection_to(*node);
	}

	//Returns how many connections this node has
	int connections_size() const {
		return connections.getSize();
	}

	//Returns the start of the graph_node list
	auto begin()
	{
		return iterator(&connections, connections.begin());
	}

	//Returns the start of the graph_node list
	auto cbegin() const
	{
		return const_iterator(&connections, connections.cbegin());
	}

	//Returns the start of the graph_node list
	auto begin() const
	{
		return cbegin();
	}

	//Returns the end of the graph_node list
	auto end()
	{
		return iterator(&connections, connections.end());
	}

	//Returns the end of the graph_node list
	auto cend() const
	{
		return const_iterator(&connections, connections.cend());
	}

	//Returns the end of the graph_node list
	auto end() const
	{
		return cend();
	}

	bool operator!=(const graph_node<T>& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator==(const graph_node<T>& rhs) const
	{
		if (connections.getSize() != rhs.connections.getSize())
		{
			return false;
		}

		auto rhsBegin = rhs.connections.begin();

		//Check if the connection values are equal
		for (auto i = connections.begin(); i != connections.end(); i++) {
			if ((*i)->value != (*rhsBegin)->value) {
				return false;
			}
			++rhsBegin;
		}

		return value == rhs.value;
	}
};

//Represents a collection of nodes that are connected to each other
template<typename T>
class graph {
	//The list of all the nodes in the graph
	linked_list<graph_node<T>> nodes{};
public:

	using node = graph_node<T>;

	using iterator = typename linked_list<graph_node<T>>::iterator; //const_iterator
	using const_iterator = typename linked_list<graph_node<T>>::const_iterator;

	//The default constructor for the graph
	graph() = default;

	//The copy constructor for the graph
	graph(const graph<T>& toCopy) : nodes(toCopy.nodes){}

	graph(graph<T>&& toMove) noexcept : nodes(std::move(toMove.nodes)) {}

	graph<T>& operator=(const graph<T>& toCopy)
	{
		//Make a copy of the nodes list
		nodes = toCopy.nodes;
		return *this;
	}
	graph<T>& operator=(graph<T>&& toMove)
	{
		//Move the nodes list to this graph
		nodes = std::move(toMove.nodes);
		return *this;
	}

	//The equality operator
	bool operator==(const graph<T>& rhs) const
	{
		if (nodes.getSize() != rhs.size()) {
			return false;
		}

		auto iterA = nodes.begin();
		auto iterB = rhs.nodes.begin();
		for (int i = 0; i < nodes.getSize(); i++) {

			if (*iterA != *iterB) {
				return false;
			}

			++iterA;
			++iterB;
		}
		//Test if the node lists are equal
		return true;
	}

	//The inequality operator
	bool operator!=(const graph<T>& rhs) const
	{
		//Test of the node lists are not equal
		if (nodes.getSize() != rhs.size()) {
			return true;
		}

		auto iterA = nodes.begin();
		auto iterB = rhs.nodes.begin();
		for (int i = 0; i < nodes.getSize(); i++) {
			if (*iterA != *iterB) {
				return true;
			}

			++iterA;
			++iterB;
		}

		//Test if the node lists are equal
		return false;
	}

	//The destructor for the graph
	~graph() = default;

	//Returns the start of the node list
	auto begin()
	{
		//return iterator(&nodes,nodes.begin());
		return nodes.begin();
	}

	//Returns the start of the node list
	auto cbegin() const
	{
		return nodes.cbegin();
		//return const_iterator(&nodes,nodes.cbegin());
	}

	//Returns the start of the node list
	auto begin() const
	{
		return cbegin();
	}

	//Returns the end of the node list
	auto end()
	{
		return nodes.end();
		//return iterator(&nodes,nodes.end());
	}

	//Returns the end of the node list
	auto cend() const
	{
		return nodes.cend();
		//return const_iterator(&nodes,nodes.cend());
	}

	//Returns the end of the node list
	auto end() const
	{
		return cend();
	}

	//Returns how many nodes are in the graph
	auto size() const
	{
		return nodes.getSize();
	}

	//Adds a new node to the graph with the specified data
	template<typename DataType>
	iterator add_node(DataType&& data)
	{
		//Construct a new node and add it to the nodes list
		nodes.emplace_back(std::forward<DataType>(data));
		//Return a pointer to the last element in the list
		//return iterator(&nodes,--nodes.end());
		return --nodes.end();
	}

	//Deletes a node from the graph
	void delete_node(iterator iter)
	{
		//If the node is nullptr, then nothing can be done
		if (iter == end())
		{
			return;
		}
		//Looks over all the nodes in the graph and removes all connections that point to this node
		for (auto n = begin(); n != end(); n++) {
			if (n->has_connection_to(*iter)) {
				n->remove_connection_to(*iter);
			}
		}
		//Remove the node from the list of nodes

		for (auto n = nodes.begin(); n != nodes.end(); n++) {
			if (*n == *iter) {
				nodes.pop_element(n);
				return;
			}
		}
	}

	//Deletes a node from the graph by finding the node with the corresponding data
	template<typename DataType>
	void delete_node_by_value(DataType&& data)
	{
		//Find the node and attempt to delete it
		delete_node(find_node(std::forward<DataType>(data)));
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	template<typename DataType>
	iterator find_node(DataType&& dataToFind)
	{
		//Look over all the nodes
		for (auto i = nodes.begin(); i != nodes.end(); i++)
		{
			//If we found a node with the same data
			if (i->value == dataToFind)
			{
				//Return a pointer to the node
				return i;
			}
		}
		//Return a nullptr if a value was not found
		return end();
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	template<typename DataType>
	const_iterator find_node(DataType&& dataToFind) const
	{
		//Look over all the nodes
		for (auto i = nodes.begin(); i != nodes.end(); i++)
		{
			//If we found a node with the same data
			if (i->value == dataToFind)
			{
				//Return a pointer to the node
				return i;
			}
		}
		//Return a nullptr if a value was not found
		return end();
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	const_iterator find_node_by(std::function<bool(const T&)>&& delegate) const
	{
		//Look over all the nodes
		for (auto i = nodes.begin(); i != nodes.end(); i++)
		{
			//If we found a node with the same data
			if (delegate(i->value))
			{
				//Return a pointer to the node
				return i;
			}
		}
		//Return a nullptr if a value was not found
		return end();
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	iterator find_node_by(std::function<bool(const T&)>&& delegate)
	{
		//Look over all the nodes
		for (auto i = nodes.begin(); i != nodes.end(); i++)
		{
			//If we found a node with the same data
			if (delegate(i->value))
			{
				//Return a pointer to the node
				return i;
			}
		}
		return end();
	}

	//Clears the graph of all nodes
	void clear()
	{
		for (int i = size() - 1; i >= 0; i--) {
			delete_node(begin());
		}
	}
};

//Prints the graph to the console
template <typename T>
std::ostream& operator<<(std::ostream& stream, const graph_node<T>& node)
{
	//Print the connections test
	stream << "Connections -> {";


	//If there are no connections
	if (node.connections_size() == 0)
	{
		//Print a closing bracket and return
		stream << "}";
		return stream;
	}

	//Get the beginning of the connections list
	auto iter = node.begin();

	//Loop over all the values in the list except the last
	for (int i = 0; i < node.connections_size() - 1; i++)
	{
		//Print the value to the console
		stream << iter->value << ", ";
		iter++;
	}

	//Print the last value to the console and a closing bracket
	stream << iter->value << "}";

	//Return the stream
	return stream;
}

//Prints the graph to the console
template<typename T>
std::ostream& operator<<(std::ostream& stream, const graph<T>& graph)
{
	//Loop over all the nodes in the graph
	for (auto i = graph.begin(); i != graph.end(); i++)
	{
		const auto& node = *i;
		//Print the node and node value to the console
		stream << "Node {" << node.value << "}: " << node << "\n";
	}

	//Return the stream
	return stream;
}
