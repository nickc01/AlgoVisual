#pragma once
#include <type_traits>
#include <initializer_list>
#include <ostream>
#include <common.h>
#include <struct_exception.h>
#include <functional>
#include <utility>

//Represents a list of items connected to each other via pointers
template<typename T>
class linked_list {
public:
	//Represents a node in the linked list
	struct node {
		//The value of the node
		T value;
		//A pointer to the next node
		node* next;
		//A pointer to the previous node
		node* prev;

		node(const T& val, node* nextPtr, node* prevPtr) :
			value(val),
			next(nextPtr),
			prev(prevPtr) {}

		node(T&& val, node* nextPtr, node* prevPtr) :
			value(std::move(val)),
			next(nextPtr),
			prev(prevPtr) {}

		template<typename... Args>
		node(std::pair<node*, node*> ptrs, Args&&... args) :
			value(std::forward<Args>(args)...),
			next(ptrs.first),
			prev(ptrs.second) {}

		node() = default;
	};

	//Represents an iterator for iterating over all the nodes in a linked_list
	template<bool is_const>
	class node_iterator_base {

		friend node_iterator_base<!is_const>;

		using node_type = make_const_if_true<node, is_const>;
		using list_type = make_const_if_true<linked_list<T>, is_const>;

		//The node this iterator is currently accessing
		node_type* _node;
		//The list the node came from
		list_type* _list;

	public:
		using value_type = make_const_if_true<T, is_const>;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = int;

		node_iterator_base(node_type* node, list_type* list) : _node(node), _list(list) {}

		//Pre-increments the iterator to the next value
		node_iterator_base<is_const>& operator++()
		{
			//If we are at the end of the list
			if (_node == nullptr)
			{
				//We can't iterate past it
				throw struct_exception("Attempting to iterate past the end of the list");
			}

			//Move to the next node
			_node = _node->next;
			//Return the iterator
			return *this;
		}
		//Post-increments the iterator to the next value
		node_iterator_base<is_const> operator++(int)
		{
			//If we are at the end of the list
			if (_node == nullptr)
			{
				//We can't iterate past it
				throw struct_exception("Attempting to iterate past the end of the list");
			}
			//Store the current state of the iterator
			node_iterator_base<is_const> previousState = *this;
			//Move to the next node
			_node = _node->next;
			//Return the previous state of the iterator
			return previousState;
		}

		//Pre-decrements the iterator to the previous value
		node_iterator_base<is_const>& operator--()
		{
			//If the node is nullptr, then the iterator refers to the element past the end of the list
			if (_node == nullptr)
			{
				//Set this iterator to the last VALID node in the list
				_node = _list->last;

				//If the last element is null, then the list is empty
				if (_node == nullptr)
				{
					throw struct_exception("Attempting to iterate over an empty linked list");
				}
			}
			//If we are somewhere in the middle of the list
			else
			{
				//If there is a previous element
				if (_node->prev != nullptr)
				{
					//Set the node to the previous element
					_node = _node->prev;
				}
				//If there is no previous element, then we are at the beginning already
				else
				{
					throw struct_exception("Attempting to iterate past the beginning of the list");
				}
			}
			//Return a reference to the iterator
			return *this;
		}
		//Post-decrements the iterator to the previous value
		node_iterator_base<is_const> operator--(int)
		{
			//Store the current state of the iterator
			node_iterator_base<is_const> previousState = *this;
			//If the node is nullptr, then we are at the end of the list
			if (_node == nullptr)
			{
				//Set this iterator to the last VALID node in the list
				_node = _list->last;
				//If the last element is null, then the list is empty
				if (_node == nullptr)
				{
					throw struct_exception("Attempting to iterate over an empty linked list");
				}
			}
			//If we are somewhere in the middle of the list
			else
			{
				//If there is a previous element
				if (_node->prev != nullptr)
				{
					//Set the node to the previous element
					_node = _node->prev;
				}
				//If there is no previous element, then we are at the beginning already
				else
				{
					throw struct_exception("Attempting to iterate past the beginning of the list");
				}
			}
			//Return the previous state of the iterator
			return previousState;
		}

		//Used to get the value of the iterator
		reference operator*()
		{
			//Return a reference to the value
			return _node->value;
		}
		//Used for dereferencing the value
		pointer operator->()
		{
			//Return a pointer to the value
			return &_node->value;
		}

		node_type* get_node() const {
			return _node;
		}

		//Tests for equality
		template <bool other_const>
		bool operator==(const node_iterator_base<other_const>& rhs) const
		{
			return rhs._node == _node && rhs._list == _list;
		}
		//Tests for inequality
		template <bool other_const>
		bool operator!=(const node_iterator_base<other_const>& rhs) const
		{
			return rhs._node != _node || rhs._list != _list;
		}
	};

	using const_node_iterator = node_iterator_base<true>;
	using node_iterator = node_iterator_base<false>;

	using iterator = node_iterator;
	using const_iterator = const_node_iterator;

private:
	//The first node in the linked list
	node* first = nullptr;
	//The last node in the linked list
	node* last = nullptr;
	//How many nodes are in the linked list
	int size = 0;

	//Inserts a new node. The node will be inserted before the "elementToInsertBefore" iterator
	node_iterator insert(node* newNode, const node_iterator elementToInsertBefore)
	{
		//If the iterator is equal to the end() iterator
		if (elementToInsertBefore.get_node() == nullptr)
		{
			//If there is a last element
			if (last != nullptr)
			{
				//Set the last element's next ptr to the new node
				last->next = newNode;
				//Set the new node's previous ptr to the last element
				newNode->prev = last;
			}
			//Set the last element to be the new node
			last = newNode;
			//If there is no first element
			if (first == nullptr)
			{
				//Set the new node to be the first element too
				first = newNode;
			}
			//Increase the size of the list
			size++;
		}
		//If the iterator is not the end() iterator
		else
		{
			//If the specified element has a previous node
			if (elementToInsertBefore.get_node()->prev != nullptr)
			{
				//Get the previous node
				auto previousNode = elementToInsertBefore.get_node()->prev;

				//Set the previous node's next ptr to the new node
				previousNode->next = newNode;
				//Set the new node's previous ptr to the previous node
				newNode->prev = previousNode;
			}
			//Otherwise, the only node that wouldn't have a previous node is the beginning node, so that needs to be updated
			else
			{
				first = newNode;
			}
			//Get the specified element node
			auto nextNode = elementToInsertBefore.get_node();

			//Set the element's previous ptr to the new node
			nextNode->prev = newNode;
			//Set the new node's next ptr to the element node
			newNode->next = nextNode;
			//Increase the size of the list
			size++;
		}
		//return a new iterator that points to the new node
		return node_iterator(newNode, this);
	}

public:
	linked_list() {}
	//Constructs a linked list from a list of items
	linked_list(std::initializer_list<T> list) {
		for (auto& i : list)
		{
			push_back(i);
		}
	}

	linked_list(const linked_list<T>& copy)
	{
		clear();
		for (auto value : copy)
		{
			push_back(value);
		}
	}

	linked_list(linked_list<T>&& move) noexcept :
		first(std::move(move.first)),
		last(std::move(move.last)),
		size(std::move(move.size))
	{
		move.first = nullptr;
		move.last = nullptr;
		move.size = 0;
	}

	linked_list<T>& operator=(const linked_list<T>& copy)
	{
		clear();
		for (auto value : copy)
		{
			push_back(value);
		}
		return *this;
	}

	linked_list<T>& operator=(linked_list<T>&& move) = default;

	~linked_list()
	{
		clear();
	}

	//Clears the linked list
	void clear()
	{
		node* currentNode = first;
		while (currentNode != nullptr)
		{
			node* previousNode = currentNode;
			currentNode = currentNode->next;
			delete previousNode;
		}
		first = nullptr;
		last = nullptr;
		size = 0;
	}

	//Gets an iterator to the first node
	node_iterator begin() {
		return node_iterator(first, this);
	}

	//Gets an iterator to the node after the last node
	node_iterator end(){
		return node_iterator(nullptr, this);
	}

	//Gets an iterator to the first node
	const_node_iterator begin() const {
		return const_node_iterator(first, this);
	}

	//Gets an iterator to the node after the last node
	const_node_iterator end() const {
		return const_node_iterator(nullptr, this);
	}

	//Gets an iterator to the first node
	const_node_iterator cbegin() const {
		return const_node_iterator(first, this);
	}

	//Gets an iterator to the node after the last node
	const_node_iterator cend() const {
		return const_node_iterator(nullptr, this);
	}

	//Gets an iterator to the first node
	node_iterator front() {
		return node_iterator(first, this);
	}

	//Gets an iterator to the first node
	const_node_iterator front() const {
		return const_node_iterator(first, this);
	}

	//Gets an iterator to the last node
	node_iterator back() {
		return node_iterator(last, this);
	}

	//Gets an iterator to the last node
	const_node_iterator back() const {
		return const_node_iterator(last, this);
	}

	//Adds a node to the front of the linked list
	node_iterator push_front(const T& value)
	{
		//Construct the new node
		node* newNode = new node(value, first, nullptr);
		if (first != nullptr)
		{
			first->prev = newNode;
		}

		if (last == nullptr)
		{
			last = newNode;
		}

		first = newNode;
		++size;
		return node_iterator(first, this);
	}

	//Adds a node to the front of the linked list
	node_iterator push_front(T&& value)
	{
		//Construct the new node
		node* newNode = new node(std::move(value), first, nullptr);
		if (first != nullptr)
		{
			first->prev = newNode;
		}

		if (last == nullptr)
		{
			last = newNode;
		}

		first = newNode;
		++size;
		return node_iterator(first, this);
	}

	//Adds a node to the back of the linked list
	node_iterator push_back(const T& value)
	{
		//Construct the new node
		node* newNode = new node(value, nullptr, last);
		if (last != nullptr)
		{
			last->next = newNode;
		}

		if (first == nullptr)
		{
			first = newNode;
		}

		last = newNode;
		++size;
		return node_iterator(last, this);
	}

	//Adds a node to the back of the linked list
	node_iterator push_back(T&& value)
	{
		//Construct the new node
		node* newNode = new node(std::move(value), nullptr, last);
		if (last != nullptr)
		{
			last->next = newNode;
		}

		if (first == nullptr)
		{
			first = newNode;
		}

		last = newNode;
		++size;
		return node_iterator(last, this);
	}

	//Constructs a new node to the front of the linked list
	template<typename... Args>
	node_iterator emplace_front(T&& arguments...)
	{
		//Construct the new node
		node* newNode = new node(std::make_pair(first, nullptr), std::forward<Args>(arguments)...);
		if (first != nullptr)
		{
			first->prev = newNode;
		}

		if (last == nullptr)
		{
			last = newNode;
		}

		first = newNode;
		++size;
		return node_iterator(first, this);
	}

	//Constructs a new node to the back of the linked list
	template<typename... Args>
	node_iterator emplace_back(Args&& ...arguments)
	{
		//Construct the new node
		node* newNode = new node(std::make_pair(nullptr, last), std::forward<Args>(arguments)...);
		if (last != nullptr)
		{
			last->next = newNode;
		}

		if (first == nullptr)
		{
			first = newNode;
		}

		last = newNode;
		++size;
		return node_iterator(last, this);
	}
	
	//Removes the first element from the linked list. Returns true if it was removed successfully
	bool pop_front()
	{
		if (first != nullptr)
		{
			if (last == first)
			{
				last = nullptr;
			}

			if (first->next != nullptr)
			{
				first->next->prev = nullptr;

				auto old = first;
				first = first->next;
				delete old;
			}
			else
			{
				delete first;
				first = nullptr;
			}

			
			--size;
			return true;
		}
		return false;
	}

	//Removes the last element from the linked list. Returns true if it was removed successfully
	bool pop_back()
	{
		if (last != nullptr)
		{
			if (first == last)
			{
				first = nullptr;
			}

			if (last->prev != nullptr)
			{
				last->prev->next = nullptr;

				auto old = last;
				last = last->prev;
				delete old;
			}
			else
			{
				delete last;
				last = nullptr;
			}
			
			--size;
			return true;
		}
		return false;
	}

	//Gets how many nodes are in the linked list
	int getSize() const
	{
		return size;
	}

	//Inserts a new element before the specified position. Returns an iterator to the new node
	node_iterator insert(const T& value, const node_iterator elementToInsertBefore)
	{
		//The new node to insert
		node* newNode = nullptr;

		try
		{
			//Allocate the node
			newNode = new node(value, nullptr, nullptr);
			//Insert the node before the specified position
			return insert(newNode, elementToInsertBefore);
		}
		//If any error occurs
		catch (...)
		{
			//if the new node has been allocated
			if (newNode != nullptr)
			{
				//Delete it to avoid a memory leak
				delete newNode;
			}
			//Rethrow the exception
			throw;
		}
	}

	//Inserts a new element before the specified position by moving the value. Returns an iterator to the new node
	node_iterator insert(T&& value, const node_iterator elementToInsertBefore)
	{
		//The new node to insert
		node* newNode = nullptr;

		try
		{
			//Allocate the node
			newNode = new node(std::move(value), nullptr, nullptr);
			//Insert the node before the specified position
			return insert(newNode, elementToInsertBefore);
		}
		//If any error occurs
		catch (...)
		{
			//if the new node has been allocated
			if (newNode != nullptr)
			{
				//Delete it to avoid a memory leak
				delete newNode;
			}
			//Rethrow the exception
			throw;
		}
	}

	//Deletes an element at the specified position
	void pop_element(const node_iterator elementToRemove)
	{
		//If the elementToRemove is the same as the end() iterator
		if (elementToRemove.get_node() == nullptr)
		{
			//Cannot delete the end() iterator, since that doesn't have a valid value
			throw struct_exception("The passed in iterator does not point to a valid element");
		}
		//Get the node of the element to remove
		node* node = elementToRemove.get_node();

		//If the node is the first element
		if (node == first)
		{
			//Make the first element point to the node after the first element
			first = node->next;
		}

		//If the node is the last element
		if (node == last)
		{
			//Make the last element point to the node previous to the last
			last = node->prev;
		}

		//If the node has a previous node
		if (node->prev != nullptr)
		{
			//Set the next ptr of the previous node to the one after the current node
			node->prev->next = node->next;
		}
		//If the node has a next node
		if (node->next != nullptr)
		{
			//Set the previous ptr of the next node to the one before the current node
			node->next->prev = node->prev;
		}
		//Decrease the size
		size--;
		//Delete the current node
		delete node;
	}

	//Finds a node with the specified value
	const_node_iterator find(const T& value) const 
	{
		//Search the entire list for the specified value
		for (auto i = begin(); i != end(); i++)
		{
			if (*i == value)
			{
				return i;
			}
		}
		return end();
	}
	
	//Finds a node with the specified value
	const_node_iterator find(T&& value) const
	{
		//Search the entire list for the specified value
		for (auto i = begin(); i != end(); i++)
		{
			if (*i == value)
			{
				return i;
			}
		}
		return end();
	}

	//Finds a node with the specified value
	node_iterator find(const T& value)
	{
		//Search the entire list for the specified value
		for (auto i = begin(); i != end(); i++)
		{
			if (*i == value)
			{
				return i;
			}
		}
		return end();
	}

	//Finds a node with the specified value
	node_iterator find(T&& value)
	{
		//Search the entire list for the specified value
		for (auto i = begin(); i != end(); i++)
		{
			if (*i == value)
			{
				return i;
			}
		}
		return end();
	}

	//Tests for equality
	bool operator==(const linked_list<T>& rhs) const
	{
		if (size != rhs.size)
		{
			return false;
		}

		auto otherI = rhs.begin();

		for (auto i = begin(); i != end(); i++)
		{
			if (*i != *otherI)
			{
				return false;
			}
			++otherI;
		}

		return true;
	}

	//Tests for inequality
	bool operator!=(const linked_list<T>& rhs) const
	{
		return !(*this == rhs);
	}
};


//Gets the maximum value in the list. Returns end() if the list is empty
template<typename T, typename Comparer = decltype(sorting_impl::DefaultComparer<T>)>
typename linked_list<T>::const_node_iterator maximum(const linked_list<T>& list, Comparer& comparer = sorting_impl::DefaultComparer<T>)
{
	//The max element stores the largest value. If the list is empty, it defaults to the end iterator
	auto maxElement = list.end();
	const auto endElement = list.end();

	//Loop over all the nodes in the list
	for (auto i = list.begin(); i != list.end(); i++)
	{
		//If the maxElement is the end() or a larger value has been found
		if (maxElement == endElement || comparer(*maxElement, *i))
		{
			//Set the maxElement iterator to the current iterator
			maxElement = i;
		}
	}

	//Return the max element
	return maxElement;
}

//Gets the minimum value in the list. Returns end() if the list is empty
template<typename T, typename Comparer = decltype(sorting_impl::DefaultComparer<T>)>
typename linked_list<T>::const_node_iterator minimum(const linked_list<T>& list, Comparer& comparer = sorting_impl::DefaultComparer<T>)
{
	//The min element stores the smallest value. If the list is empty, it defaults to the end iterator
	auto minElement = list.end();
	const auto endElement = list.end();

	//Loop over all the nodes in the list
	for (auto i = list.begin(); i != list.end(); i++)
	{
		//If the minElement is the end() or a smaller value has been found
		if (minElement == endElement || comparer(*i, *minElement))
		{
			//Set the minElement iterator to the current iterator
			minElement = i;
		}
	}

	//Return the min element
	return minElement;
}

//Gets the maximum value in the list. Returns end() if the list is empty
template<typename T, typename Comparer = decltype(sorting_impl::DefaultComparer<T>)>
typename linked_list<T>::node_iterator maximum(linked_list<T>& list, Comparer& comparer = sorting_impl::DefaultComparer<T>)
{
	//The max element stores the largest value. If the list is empty, it defaults to the end iterator
	auto maxElement = list.end();
	const auto endElement = list.end();

	//Loop over all the nodes in the list
	for (auto i = list.begin(); i != list.end(); i++)
	{
		//If the maxElement is the end() or a larger value has been found
		if (maxElement == endElement || comparer(*maxElement , *i))
		{
			//Set the maxElement iterator to the current iterator
			maxElement = i;
		}
	}

	//Return the max element
	return maxElement;
}

//Gets the minimum value in the list. Returns end() if the list is empty
template<typename T, typename Comparer = decltype(sorting_impl::DefaultComparer<T>)>
typename linked_list<T>::node_iterator minimum(linked_list<T>& list, Comparer& comparer = sorting_impl::DefaultComparer<T>)
{
	//The min element stores the smallest value. If the list is empty, it defaults to the end iterator
	auto minElement = list.end();
	const auto endElement = list.end();

	//Loop over all the nodes in the list
	for (auto i = list.begin(); i != list.end(); i++)
	{
		//If the minElement is the end() or a smaller value has been found
		if (minElement == endElement || comparer(*i, *minElement))
		{
			//Set the minElement iterator to the current iterator
			minElement = i;
		}
	}

	//Return the min element
	return minElement;
}

//Used for printing a linked_list to the console
template<typename T>
std::ostream& operator<<(std::ostream& os, const linked_list<T> list) {
	if (list.getSize() == 0) {
		os << "[]";
	}
	else {
		os << '[';

		auto lastElement = --list.cend();

		for (auto i = list.cbegin(); i != list.cend(); i++) {
			if (i == lastElement) {
				os << *i;
			}
			else {
				os << *i << ", ";
			}
		}

		os << ']';
	}

	return os;
}
