#pragma once

#include <list>
#include <ostream>
#include <struct_exception.h>


//Represents a weighted, directed graph. Nodes can be added to the graph, and they can be connected to each other
//The graph is weighted, which means that connections/edges between nodes can have weights to them
//The graph is directed, meaning that each node can have a one-way connection to other nodes
template<typename T>
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
		//Throws an exception if there is already a connection to the neighbor node
		//Throws an exception if you are trying to add a connection to itself
		connection* add_connection(node* const neighbor, const unsigned int connectionWeight = 1)
		{
			//If there is already a connection to this neighbor, throw an exception
			if (has_connection_to(neighbor))
			{
				throw struct_exception("There is already a connection to the node");
			}
			//if you are trying to make a connection to itself, throw an exception
			if (neighbor == this)
			{
				throw struct_exception("Cannot have a connection to itself");
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
		nodes.push_back(node(this, std::forward<DataType>(data)));
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
}