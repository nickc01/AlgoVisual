#pragma once
#include <type_traits>
#include <initializer_list>
#include <ostream>

template<typename T>
struct node {
	T value;
	node<T>* next;
	node<T>* prev;

	node(const T& val, node<T>* nextPtr, node<T>* prevPtr) :
		value(val),
		next(nextPtr),
		prev(prevPtr) {}

	node(T&& val, node<T>* nextPtr, node<T>* prevPtr) :
		value(std::move(val)),
		next(nextPtr),
		prev(prevPtr) {}

	node() = default;
};

template<typename T>
class linked_list;

template<typename T, bool is_const = false>
class node_iterator {
	template<typename inputType>
	using make_const_if_true = typename std::conditional<is_const, const inputType, inputType>::type;

	using node_type = make_const_if_true<node<T>>;
	using list_type = make_const_if_true<linked_list<T>>;

	node_type* _node;
	list_type* _list;

public:
	using value_type = make_const_if_true<T>;
	using reference = value_type&;
	using pointer = value_type*;
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = int;

	node_iterator(node_type* node, list_type* list) : _node(node), _list(list) {}

	//Pre-increments the iterator to the next value
	node_iterator<T, is_const>& operator++()
	{
		//If we are at the end of the list
		if (_node == nullptr)
		{
			//We can't iterate past it
			throw std::exception("Attempting to iterate past the end of the list");
		}

		//Move to the next node
		_node = _node->next;
		//Return the iterator
		return *this;
	}
	//Post-increments the iterator to the next value
	node_iterator<T, is_const> operator++(int)
	{
		//If we are at the end of the list
		if (_node == nullptr)
		{
			//We can't iterate past it
			throw std::exception("Attempting to iterate past the end of the list");
		}
		//Store the current state of the iterator
		node_iterator<T, is_const> previousState = *this;
		//Move to the next node
		_node = _node->next;
		//Return the previous state of the iterator
		return previousState;
	}

	//Pre-decrements the iterator to the previous value
	node_iterator<T, is_const>& operator--()
	{
		//If the node is nullptr, then the iterator refers to the element past the end of the list
		if (_node == nullptr)
		{
			//Set this iterator to the last VALID iterator in the list
			//*this = --_list->end();
			*this = _list->back();

			//If the last element is null, then the list is empty
			if (_node == nullptr)
			{
				throw std::exception("Attempting to iterate over an empty linked list");
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
				throw std::exception("Attempting to iterate past the beginning of the list");
			}
		}
		//Return a reference to the iterator
		return *this;
	}
	//Post-decrements the iterator to the previous value
	node_iterator<T, is_const> operator--(int)
	{
		//Store the current state of the iterator
		node_iterator<T, is_const> previousState = *this;
		//If the node is nullptr, then we are at the end of the list
		if (_node == nullptr)
		{
			//Set this iterator to the last VALID iterator in the list
			*this = _list->back();
			//If the last element is null, then the list is empty
			if (_node == nullptr)
			{
				throw std::exception("Attempting to iterate over an empty linked list");
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
				throw std::exception("Attempting to iterate past the beginning of the list");
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
	bool operator==(const node_iterator<T,is_const>& rhs) const
	{
		return rhs._node == _node && rhs._list == _list;
	}
	//Tests for inequality
	bool operator!=(const node_iterator<T, is_const>& rhs) const
	{
		return rhs._node != _node || rhs._list != _list;
	}
};

template<typename T>
using const_node_iterator = node_iterator<T, true>;

template<typename T>
class linked_list {
	node<T>* first = nullptr;
	node<T>* last = nullptr;
	int size = 0;

	//Inserts a new node. The node will be inserted before the "elementToInsertBefore" iterator
	node_iterator<T> insert(node<T>* newNode, const node_iterator<T> elementToInsertBefore)
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
		return node_iterator<T>(newNode, this);
	}

public:
	linked_list() {}
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

	linked_list(linked_list<T>&& move) noexcept = default;

	linked_list<T>& operator=(const linked_list<T>& copy)
	{
		clear();
		for (auto value : copy)
		{
			push_back(value);
		}
	}

	linked_list<T>& operator=(linked_list<T>&& move) = default;

	~linked_list()
	{
		clear();
	}

	void clear()
	{
		node<T>* currentNode = first;
		while (currentNode != nullptr)
		{
			node<T>* previousNode = currentNode;
			currentNode = currentNode->next;
			delete previousNode;
		}
		first = nullptr;
		last = nullptr;
		size = 0;
	}

	node_iterator<T> begin() {
		return node_iterator<T>(first, this);
	}

	node_iterator<T> end(){
		return node_iterator<T>(nullptr, this);
	}


	const_node_iterator<T> begin() const {
		return const_node_iterator<T>(first, this);
	}

	const_node_iterator<T> end() const {
		return const_node_iterator<T>(nullptr, this);
	}


	const_node_iterator<T> cbegin() const {
		return const_node_iterator<T>(first, this);
	}

	const_node_iterator<T> cend() const {
		return const_node_iterator<T>(nullptr, this);
	}


	node_iterator<T> front() {
		return node_iterator<T>(first, this);
	}

	const_node_iterator<T> front() const {
		return const_node_iterator<T>(first, this);
	}


	node_iterator<T> back() {
		return node_iterator<T>(last, this);
	}

	const_node_iterator<T> back() const {
		return const_node_iterator<T>(last, this);
	}

	node_iterator<T> push_front(const T& value)
	{
		node<T>* newNode = new node<T>(value, first, nullptr);
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
		return node_iterator<T>(first, this);
	}

	node_iterator<T> push_front(T&& value)
	{
		node<T>* newNode = new node<T>(std::move(value), first, nullptr);
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
		return node_iterator<T>(first, this);
	}

	node_iterator<T> push_back(const T& value)
	{
		node<T>* newNode = new node<T>(value, nullptr, last);
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
		return node_iterator<T>(last, this);
	}

	node_iterator<T> push_back(T&& value)
	{
		node<T>* newNode = new node<T>(std::move(value), nullptr, last);
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
		return node_iterator<T>(last, this);
	}

	void remove(node_iterator<T> node)
	{
		
	}

	bool removeBegin()
	{
		if (first != nullptr)
		{
			if (last == first)
			{
				last == nullptr;
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

	bool removeEnd()
	{
		if (last != nullptr)
		{
			if (first == last)
			{
				first == nullptr;
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

	int getSize() const
	{
		return size;
	}

	//Inserts a new element before the specified position. Returns an iterator to the new node
	node_iterator<T> insert(const T& value, const node_iterator<T> elementToInsertBefore)
	{
		//The new node to insert
		node<T>* newNode = nullptr;

		try
		{
			//Allocate the node
			newNode = new node<T>(value, nullptr, nullptr);
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
	node_iterator<T> insert(T&& value, const node_iterator<T> elementToInsertBefore)
	{
		//The new node to insert
		node<T>* newNode = nullptr;

		try
		{
			//Allocate the node
			newNode = new node<T>(std::move(value), nullptr, nullptr);
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
	void delete_element(const node_iterator<T> elementToRemove)
	{
		//If the elementToRemove is the same as the end() iterator
		if (elementToRemove.get_node() == nullptr)
		{
			//Cannot delete the end() iterator, since that doesn't have a valid value
			throw std::exception("The passed in iterator does not point to a valid element");
		}
		//Get the node of the element to remove
		node<T>* node = elementToRemove.get_node();

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
};

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