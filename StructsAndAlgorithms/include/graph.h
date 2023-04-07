#pragma once

#include "common.h"
#include <list>
#include <ostream>
#include <struct_exception.h>
#include <unordered_set>

template<typename T>
class graph2 {
public:
	class node {
		std::unordered_set<node*> connections;
	public:
		template<bool is_const>
		class connection_iterator_base {
			make_const_if_true<std::unordered_set<node*>,is_const>* connections = nullptr;
			decltype(connections->begin()) iter;
		public:
			using value_type = make_const_if_true<node, is_const>;
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

			connection_iterator_base<is_const>& operator++()
			{
				++iter;
				return *this;
			}

			connection_iterator_base<is_const> operator++(int)
			{
				connection_iterator_base<is_const> previous = *this;

				this->operator++();

				return previous;
			}

			/*connection_iterator_base<is_const>& operator--()
			{
				--iter;
				return *this;
			}

			connection_iterator_base<is_const> operator--(int)
			{
				connection_iterator_base<is_const> previous = *this;
				
				this->operator--();

				return previous;
			}*/

			const value_type& operator*() const
			{
				//Return a reference to the value
				return **iter;
			}
			//Used for dereferencing the value
			//This needs to be const because modifying the value in the tree would mess with the tree's structure
			const pointer operator->() const
			{
				//Return a pointer to the value
				return *iter;
			}

			//Tests for inequality
			bool operator!=(const connection_iterator_base<is_const>& rhs) const
			{
				//return rhs.nodePtr != nodePtr || rhs.tree != tree;
				return connections != rhs.connections || iter != rhs.iter;
			}

			//Tests for equality
			bool operator==(const connection_iterator_base<is_const>& rhs) const
			{
				//return rhs.nodePtr == nodePtr && rhs.tree == tree;
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

		T value;

		template <typename ValueType>
		node(ValueType&& _value) : value(std::forward<ValueType>(_value)) {}

		bool add_connection_to(node& node) {
			return connections.emplace(&node).second;
		}

		bool remove_connection_to(const node& node) {
			auto result = connections.end();
			for (auto i = connections.begin(); i != connections.end(); i++) {
				if (*i == &node) {
					result = i;
				}
			}
			//auto result = connections.find(&node);
			if (result != connections.end()) {
				connections.erase(result);
				return true;
			}
			return false;
		}

		void remove_all_connections() {
			connections.clear();
		}

		bool has_connection_to(const node& node) const {
			for (auto i = connections.begin(); i != connections.end(); i++) {
				if (*i == &node) {
					return true;
				}
			}
			return false;
			//return connections.find(&node) != connections.end();
		}

		int connections_size() const {
			return connections.size();
		}

		//Returns the start of the node list
		auto begin()
		{
			return iterator(&connections,connections.begin());
		}

		//Returns the start of the node list
		auto cbegin() const
		{
			return const_iterator(&connections,connections.cbegin());
		}

		//Returns the start of the node list
		auto begin() const
		{
			return cbegin();
		}

		//Returns the end of the node list
		auto end()
		{
			return iterator(&connections,connections.end());
		}

		//Returns the end of the node list
		auto cend() const
		{
			return const_iterator(&connections,connections.cend());
		}

		//Returns the end of the node list
		auto end() const
		{
			return cend();
		}
	};

private:
	//The list of all the nodes in the graph
	std::list<node> nodes{};
public:
	template<bool is_const>
	class node_iterator_base {
		make_const_if_true<std::list<node>, is_const>* source_nodes;
		decltype(source_nodes->begin()) iter;
	public:
		
		using value_type = make_const_if_true<node, is_const>;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = int;

		node_iterator_base() = default;

		node_iterator_base(decltype(source_nodes) _nodes) : source_nodes(_nodes), iter(_nodes->end()) {}

		node_iterator_base(decltype(source_nodes) _nodes, decltype(iter) _iter) : source_nodes(_nodes), iter(_iter) {}

		template<bool other_const>
		node_iterator_base(const node_iterator_base<other_const>& other) : source_nodes(other.source_nodes), iter(other.iter) {}

		template<bool other_const>
		node_iterator_base(node_iterator_base<other_const>&& other) noexcept : source_nodes(std::move(other.source_nodes)), iter(std::move(other.iter)) {}

		node_iterator_base<is_const>& operator++()
		{
			++iter;
			return *this;
		}

		node_iterator_base<is_const> operator++(int)
		{
			node_iterator_base<is_const> previous = *this;

			this->operator++();

			return previous;
		}

		node_iterator_base<is_const>& operator--()
		{
			--iter;
			return *this;
		}

		node_iterator_base<is_const> operator--(int)
		{
			node_iterator_base<is_const> previous = *this;
			
			this->operator--();

			return previous;
		}

		const value_type& operator*() const
		{
			//Return a reference to the value
			return *iter;
		}
		//Used for dereferencing the value
		//This needs to be const because modifying the value in the tree would mess with the tree's structure
		const pointer operator->() const
		{
			//Return a pointer to the value
			return iter.operator->();
		}

		//Tests for inequality
		bool operator!=(const node_iterator_base<is_const>& rhs) const
		{
			//return rhs.nodePtr != nodePtr || rhs.tree != tree;
			return source_nodes != rhs.source_nodes || iter != rhs.iter;
		}

		//Tests for equality
		bool operator==(const node_iterator_base<is_const>& rhs) const
		{
			//return rhs.nodePtr == nodePtr && rhs.tree == tree;
			return source_nodes == rhs.source_nodes && iter == rhs.iter;
		}

		//A copy assignment operator used for assigning a non-const iterator to a const version
		template<bool other_const = is_const>
		node_iterator_base<is_const>& operator=(const node_iterator_base<other_const>& other) noexcept
		{
			source_nodes = other.source_nodes;
			iter = other.iter;
			return *this;
		}

		//A move assignment operator used for assigning a non-const iterator to a const version
		template<bool other_const = is_const>
		node_iterator_base<is_const>& operator=(node_iterator_base<other_const>&& other) noexcept
		{
			source_nodes = std::move(other.source_nodes);
			iter = std::move(other.iter);
			return *this;
		}
	};

	using iterator = node_iterator_base<false>;
	using const_iterator = node_iterator_base<true>;

	//The default constructor for the graph
	graph2() = default;

	//The copy constructor for the graph
	graph2(const graph2<T>& toCopy) : nodes(toCopy.nodes)
	{
		//Update each of the nodes so that their source graph pointers are pointing to this graph
		for (auto& node : nodes)
		{
			//node.source_nodes = nodes;
		}
	}
	graph2(graph2<T>&& toMove) noexcept : nodes(std::move(toMove.nodes))
	{
		for (auto& node : nodes)
		{
			//node.source_nodes = nodes;
		}
	}

	graph2<T>& operator=(const graph2<T>& toCopy)
	{
		//Make a copy of the nodes list
		nodes = toCopy.nodes;
		//Update each of the nodes so that their source graph pointers are pointing to this graph
		/*for (auto& node : nodes)
		{
			node.sourceGraph = this;
		}*/
	}
	graph2<T>& operator=(graph2<T>&& toMove)
	{
		//Move the nodes list to this graph
		nodes = std::move(toMove.nodes);
		//Update each of the nodes so that their source graph pointers are pointing to this graph
		/*for (auto& node : nodes)
		{
			node.sourceGraph = this;
		}*/
	}

	//The equality operator
	bool operator==(const graph2<T>& rhs) const
	{
		//Test if the node lists are equal
		return nodes == rhs.nodes;
	}

	//The inequality operator
	bool operator!=(const graph2<T>& rhs) const
	{
		//Test of the node lists are not equal
		return nodes != rhs.nodes;
	}

	//The destructor for the graph
	~graph2() = default;

	//Returns the start of the node list
	auto begin()
	{
		return iterator(&nodes,nodes.begin());
	}

	//Returns the start of the node list
	auto cbegin() const
	{
		return const_iterator(&nodes,nodes.cbegin());
	}

	//Returns the start of the node list
	auto begin() const
	{
		return cbegin();
	}

	//Returns the end of the node list
	auto end()
	{
		return iterator(&nodes,nodes.end());
	}

	//Returns the end of the node list
	auto cend() const
	{
		return const_iterator(&nodes,nodes.cend());
	}

	//Returns the end of the node list
	auto end() const
	{
		return cend();
	}

	//Returns how many nodes are in the graph
	auto size() const
	{
		return nodes.size();
	}

	//Adds a new node to the graph with the specified data
	template<typename DataType>
	iterator add_node(DataType&& data)
	{
		//Construct a new node and add it to the nodes list
		nodes.emplace_back(this, std::forward<DataType>(data));
		//Return a pointer to the last element in the list
		return iterator(&nodes,--nodes.end());
	}

	//Deletes a node from the graph
	void delete_node(iterator iter)
	{
		//If the node is nullptr, then nothing can be done
		if (iter == end())
		{
			return;
		}
		//If the node is from a different graph, then nothing can be done
		/*if (node->sourceGraph != this)
		{
			return;
		}*/
		//Looks over all the nodes in the graph and removes all connections that point to this node
		for (auto n = begin(); n != end(); n++) {
			if (n->has_connection_to(*iter)) {
				n->remove_connection_to(*iter);
			}
		}
		//Remove the node from the list of nodes

		for (auto n = nodes.begin(); n != nodes.end(); n++) {
			if (*n == *iter) {
				nodes.erase(n);
				return;
			}
		}
	}

	//Deletes a node from the graph by finding the node with the corresponding data
	template<typename DataType>
	void delete_node_by_value(DataType&& data)
	{
		//Find the node and attempt to delete it
		delete_node(find_node_nonconst(std::forward<DataType>(data)));
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	template<typename DataType>
	iterator find_node_nonconst(DataType&& dataToFind)
	{
		//Look over all the nodes
		for (auto i = nodes.begin(); i != nodes.end(); i++)
		{
			/*graph2<int> testGraph;

			auto testNode = testGraph.find_node(123);

			testNode->value*/
			//If we found a node with the same data
			if (i->value == dataToFind)
			{
				//Return a pointer to the node
				return iterator(&nodes,i);
			}
		}
		//Return a nullptr if a value was not found
		return nullptr;
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	template<typename DataType>
	const_iterator find_node(DataType&& dataToFind) const
	{
		//Look over all the nodes
		//for (auto& node : nodes)
		for (auto i = nodes.begin(); i != nodes.end(); i++)
		{
			//If we found a node with the same data
			if (i->value == dataToFind)
			{
				//Return a pointer to the node
				return const_iterator(&nodes,i);
			}
		}
		//Return a nullptr if a value was not found
		return nullptr;
	}

	//Clears the graph of all nodes
	void clear()
	{
		for (int i = size() - 1; i >= 0; i--) {
			delete_node(begin());
		}
	}

	//Prints the graph to the console
	friend std::ostream& operator<<(std::ostream& stream, const node& node)
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
		auto begin = node.begin();
		//Get the last VALID iterator of the connections list
		auto end = --node.end();

		//Loop over all the values in the list except the last
		for (auto i = begin; i != end; i++)
		{
			//Print the value to the console
			stream << i->getDestinationNode()->Value << ", ";
		}

		//Print the last value to the console and a closing bracket
		stream << end->getDestinationNode()->Value << "}";

		//Return the stream
		return stream;
	}
};

//Prints the graph to the console
template<typename T>
std::ostream& operator<<(std::ostream& stream, const graph2<T>& graph)
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

//Represents a weighted, directed graph. Nodes can be added to the graph, and they can be connected to each other
//The graph is weighted, which means that connections/edges between nodes can have weights to them
//The graph is directed, meaning that each node can have a one-way connection to other nodes
/*template<typename T>
class graph
{
public:
	class node;

	//Represents a one-way connection between two nodes
	//Each connection can have a weight assigned to them, which can be utilized for algorithms such as pathfinding
	class connection
	{
		//The node that the connection is going to
		node* destination;
		//The weight of the connection
		unsigned int weight;
		//The node that the connection is coming from
		node* source;
	public:
		//The copy constructor
		connection(const connection& toCopy) = default;
		//The move constructor
		connection(connection&& toMove) noexcept = default;

		//The copy assignment operator
		connection& operator=(const connection& toCopy) = default;
		//The move assignment operator
		connection& operator=(connection&& toMove) noexcept = default;

		//The main constructor that constructs a new connection
		connection(node* destination, node* source, unsigned int weight) : destination(destination), weight(weight), source(source) {}

		//The equality operator
		bool operator==(const connection& rhs) const
		{
			//Test if each of the fields are equal
			return destination->Value == rhs.destination->Value && weight == rhs.weight && source->Value == rhs.source->Value;
		}

		//The inequality operator
		bool operator!=(const connection& rhs) const
		{
			//Test if each of the fields are not equal
			return destination->Value != rhs.destination->Value || weight != rhs.weight || source->Value != rhs.source->Value;
		}

		//Gets the destination node
		node* getDestinationNode() const
		{
			return destination;
		}

		//Gets the weight of the node
		unsigned int getWeight() const
		{
			return weight;
		}

		//Sets a new weight for the node
		void setWeight(unsigned int newWeight)
		{
			weight = newWeight;
		}

		//Gets the source node
		node* getSourceNode() const
		{
			return source;
		}
	};

	//Represents a point in the graph. Each node can have connections to other nodes to form a network
	class node
	{
		//The graph the node is a part of
		graph<T>* sourceGraph;

		//Allows the graph class to access the private fields
		friend graph<T>;

		//A list of all the connections to other nodes
		std::list<connection> Connections;

		//Looks at all the nodes in the graph and removes all connections to this particular node
		void RemoveAllConnectionsFromGraph()
		{
			//Loop over each node in the graph
			for (auto& node : sourceGraph->nodes)
			{
				//Search all the node's connections and remove connections that point to this node
				node.RemoveConnectionsToNode(this);
			}
		}

		//Removes any connections that point to the specified neighbor
		void RemoveConnectionsToNode(const node* const neighbor)
		{
			//The start of the iterator
			auto begin = Connections.begin();
			//The end of the iterator
			auto end = Connections.end();

			//Loop untill we are at the end of the iterator
			while (begin != end)
			{
				//If the destination is equal to the neighbor we are trying to remove
				if (begin->getDestinationNode() == neighbor)
				{
					//Store the previous node position
					auto previous = begin;
					//Increment the beginning iterator
					++begin;
					//Remove the neighboring connection
					Connections.remove(*previous);
				}
				//If the destination is not equal to the neighbor we are trying to remove
				else
				{
					//Increment to the next node
					++begin;
				}
			}
		}
	public:
		//The value of the node
		T Value;
	private:
		//Constructs a new node with the specified value
		node(graph<T>* const sourceGraph, const T& value) : sourceGraph(sourceGraph), Value(value) {}
		//Constructs a new node with the specified value by moving it
		node(graph<T>* const sourceGraph, T&& value) : sourceGraph(sourceGraph), Value(std::move(value)) {}
	public:

		//The copy constructor
		node(const node& toCopy) = default;
		//The move constructor
		node(node&& toMove) noexcept = default;

		//The copy assignment operator
		node& operator=(const node& toCopy) = default;
		//The move assignment operator
		node& operator=(node&& toMove) noexcept = default;

		//The equality operator
		bool operator==(const node& rhs) const
		{
			//Test if each of the fields are equal
			return Connections == rhs.Connections && Value == rhs.Value;
		}

		//The inequality operator
		bool operator!=(const node& rhs) const
		{
			//Test if any of the fields are not equivalent
			return Connections != rhs.Connections || Value != rhs.Value;
		}

		//Adds a new connection to a node with the specified weight
		//Returns a null connection if there is already a connection to the neighbor node
		//Returns a null connection  if you are trying to add a connection to itself
		connection* add_connection(node* const neighbor, const unsigned int connectionWeight = 1)
		{
			//If there is already a connection to this neighbor, throw an exception
			if (has_connection_to(neighbor))
			{
				return nullptr;
			}
			//if you are trying to make a connection to itself, throw an exception
			if (neighbor == this)
			{
				return nullptr;
			}
			//Create a new connection
			Connections.emplace_back(neighbor, this, connectionWeight);
			//Return a pointer to the connection
			return &Connections.back();
		}

		//Adds a new connection by first creating a new node in the graph, then adding a connection to it
		template<typename DataType>
		connection* add_connection(DataType data, const unsigned int connectionWeight = 1)
		{
			//Create the new node and add a connection to it
			return add_connection(sourceGraph->add_node(std::forward<DataType>(data)), connectionWeight);
		}

		//Removes an existing connection
		//Returns true if the removal was successful
		bool remove_connection(const connection*& connection)
		{
			//If the connection is null, then nothing can be done
			if (connection == nullptr)
			{
				return false;
			}
			//If the source node is the same as this node
			if (connection->getSourceNode() == this)
			{
				//Remove all connections that point to the destination node
				RemoveConnectionsToNode(connection->getDestinationNode());
				//Make the connection null
				connection = nullptr;

				//Return true for success
				return true;
			}

			//Return false for failure
			return false;
		}

		//Removes an existing connection by finding the node with the corresponding data value, then removing the connection to it
		//Returns true if the removal was successful, and false otherwise
		template<typename DataType>
		bool remove_connection(DataType&& dataToRemove)
		{
			return remove_connection(sourceGraph->find_node(dataToRemove));
		}

		//Returns true if the node has a connection to the specified neighbor
		bool has_connection_to(node* neighbor) const
		{
			//Loop over all the added connections
			for (auto& connection : Connections)
			{
				//If the connection points to the specified neighbor
				if (connection.getDestinationNode() == neighbor)
				{
					//Return true since a neighbor was found
					return true;
				}
			}
			//Return false since no neighbor was found
			return false;
		}

		bool has_connection_to(const node& neighbor) const
		{
			return has_connection_to(&neighbor);
		}

		//Returns the list of all connections
		const std::list<connection>& get_connections() const
		{
			return Connections;
		}

		//Prints the contents of the node to the console
		//Friend is required in this case, since the "<<" operator can only be a non-member function, and friend in this instance makes the function a non-member function
		friend std::ostream& operator<<(std::ostream& stream, const node& node)
		{
			//Print the connections test
			stream << "Connections -> {";

			//Get the connections
			auto connections = node.get_connections();

			//If there are no connections
			if (connections.size() == 0)
			{
				//Print a closing bracket and return
				stream << "}";
				return stream;
			}

			//Get the beginning of the connections list
			auto begin = connections.begin();
			//Get the last VALID iterator of the connections list
			auto end = --connections.end();

			//Loop over all the values in the list except the last
			for (auto i = begin; i != end; i++)
			{
				//Print the value to the console
				stream << i->getDestinationNode()->Value << ", ";
			}

			//Print the last value to the console and a closing bracket
			stream << end->getDestinationNode()->Value << "}";

			//Return the stream
			return stream;
		}

		//Gets the graph the node is a part of
		graph<T>* GetSourceGraph() const
		{
			return sourceGraph;
		}


	};

private:
	//The list of all the nodes in the graph
	std::list<node> nodes;
public:

	//The default constructor for the graph
	graph() {}

	//The copy constructor for the graph
	graph(const graph<T>& toCopy)
	{
		//Make a copy of the nodes list
		nodes = toCopy.nodes;

		//Update each of the nodes so that their source graph pointers are pointing to this graph
		for (auto& node : nodes)
		{
			node.sourceGraph = this;
		}
	}
	graph(graph<T>&& toMove) noexcept
	{
		//Move the nodes list to this graph
		nodes = std::move(toMove.nodes);
		//Update each of the nodes so that their source graph pointers are pointing to this graph
		for (auto& node : nodes)
		{
			node.sourceGraph = this;
		}
	}

	graph<T>& operator=(const graph<T>& toCopy)
	{
		//Make a copy of the nodes list
		nodes = toCopy.nodes;
		//Update each of the nodes so that their source graph pointers are pointing to this graph
		for (auto& node : nodes)
		{
			node.sourceGraph = this;
		}
	}
	graph<T>& operator=(graph<T>&& toMove)
	{
		//Move the nodes list to this graph
		nodes = std::move(toMove.nodes);
		//Update each of the nodes so that their source graph pointers are pointing to this graph
		for (auto& node : nodes)
		{
			node.sourceGraph = this;
		}
	}

	//The equality operator
	bool operator==(const graph<T>& rhs) const
	{
		//Test if the node lists are equal
		return nodes == rhs.nodes;
	}

	//The inequality operator
	bool operator!=(const graph<T>& rhs) const
	{
		//Test of the node lists are not equal
		return nodes != rhs.nodes;
	}

	//The destructor for the graph
	~graph() = default;

	//Adds a new node to the graph with the specified data
	template<typename DataType>
	node* add_node(DataType&& data)
	{
		//Construct a new node and add it to the nodes list
		nodes.emplace_back(this, std::forward<DataType>(data));
		//Return a pointer to the last element in the list
		return &nodes.back();
	}

	//Deletes a node from the graph
	void delete_node(node* node)
	{
		//If the node is nullptr, then nothing can be done
		if (node == nullptr)
		{
			return;
		}
		//If the node is from a different graph, then nothing can be done
		if (node->sourceGraph != this)
		{
			return;
		}

		//Looks over all the nodes in the graph and removes all connections that point to this node
		node->RemoveAllConnectionsFromGraph();
		//Remove the node from the list of nodes
		nodes.remove(*node);
	}

	//Deletes a node from the graph by finding the node with the corresponding data
	template<typename DataType>
	void delete_node(DataType&& data)
	{
		//Find the node and attempt to delete it
		delete_node(find_node(std::forward<DataType>(data)));
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	template<typename DataType>
	node* find_node(DataType&& dataToFind)
	{
		//Look over all the nodes
		for (auto& node : nodes)
		{
			//If we found a node with the same data
			if (node.Value == dataToFind)
			{
				//Return a pointer to the node
				return &node;
			}
		}
		//Return a nullptr if a value was not found
		return nullptr;
	}

	//Finds a node in the graph with the corresponding data
	//Returns nullptr if the node cannot be found
	template<typename DataType>
	const node* find_node(DataType&& dataToFind) const
	{
		//Look over all the nodes
		for (auto& node : nodes)
		{
			//If we found a node with the same data
			if (node.Value == dataToFind)
			{
				//Return a pointer to the node
				return &node;
			}
		}
		//Return a nullptr if a value was not found
		return nullptr;
	}

	//Returns the start of the node list
	auto begin()
	{
		return nodes.begin();
	}

	//Returns the start of the node list
	auto cbegin() const
	{
		return nodes.cbegin();
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
	}

	//Returns the end of the node list
	auto cend() const
	{
		return nodes.cend();
	}

	//Returns the end of the node list
	auto end() const
	{
		return cend();
	}

	//Returns how many nodes are in the graph
	auto size() const
	{
		return nodes.size();
	}

	//Clears the graph of all nodes
	void clear()
	{
		nodes.clear();
	}
};

//Prints the graph to the console
template<typename T>
std::ostream& operator<<(std::ostream& stream, const graph<T>& graph)
{
	//Loop over all the nodes in the graph
	for (auto i = graph.begin(); i != graph.end(); i++)
	{
		const auto& node = *i;
		//Print the node and node value to the console
		stream << "Node {" << node.Value << "}: " << node << "\n";
	}

	//Return the stream
	return stream;
}*/