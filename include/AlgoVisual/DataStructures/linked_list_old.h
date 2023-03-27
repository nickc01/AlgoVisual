/*#include <type_traits>
#include <exception>
#include <algorithm>
#include <ostream>

//template <typename T>
//class linked_list;

//template <typename T>



//The iterator for the doubly linked list
    //This iterator has all the properties necessary for a bidirectional iterator
    //http://www.cplusplus.com/reference/iterator/BidirectionalIterator/
//template<typename T>


template <typename T>
class linked_list
{
public:
    struct linked_list_node
    {
        T value; //The value the node represents
        std::shared_ptr<linked_list_node> next; //The next node in the linked list
        std::shared_ptr<linked_list_node> previous; //The previous node in the linked list

        //Constructs a new node based on a value
        explicit linked_list_node(const T& value, std::shared_ptr<linked_list_node> next = nullptr, std::shared_ptr<linked_list_node> previous = nullptr) : value(value), next(next), previous(previous) {}

        //Constructs a new node by moving an existing value
        explicit linked_list_node(T&& value, std::shared_ptr<linked_list_node> next = nullptr, std::shared_ptr<linked_list_node> previous = nullptr) : value(std::move(value)), next(next), previous(previous) {}
    };

    class linked_list_iterator
    {
        //Allows the doubly linked list to access the node of the iterator
        friend class linked_list<T>;

        //The node that the iterator points to
        std::shared_ptr<linked_list_node> node;

        //The doubly linked list the iterator is a part of
        const std::shared_ptr<linked_list<T>> list;
    public:

        //These type definitions are required for iterators
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = int;

        //Constructs a new iterator from a node
        linked_list_iterator(std::shared_ptr<linked_list_node> node, const std::shared_ptr<linked_list<T>> list) : node(node), list(list) { }

        //Pre-increments the iterator to the next value
        linked_list_iterator& operator++()
        {
            //If we are at the end of the list
            if (node == nullptr)
            {
                //We can't iterate past it
                throw std::exception("Attempting to iterate past the end of the list");
            }

            //Move to the next node
            node = node->next;
            //Return the iterator
            return *this;
        }
        //Post-increments the iterator to the next value
        linked_list_iterator operator++(int)
        {
            //If we are at the end of the list
            if (node == nullptr)
            {
                //We can't iterate past it
                throw std::exception("Attempting to iterate past the end of the list");
            }
            //Store the current state of the iterator
            linked_list_iterator previousState = *this;
            //Move to the next node
            node = node->next;
            //Return the previous state of the iterator
            return previousState;
        }

        //Pre-decrements the iterator to the previous value
        linked_list_iterator& operator--()
        {
            //If the node is nullptr, then the iterator refers to the element past the end of the list
            if (node == nullptr)
            {
                //Get the last element in the linked list
                node = list->lastElement;

                //node = std::make_shared<linked_list_node>(--list->end());
                //If the last element is null, then the list is empty
                if (node == nullptr)
                {
                    throw std::exception("Attempting to iterate over an empty linked list");
                }
            }
            //If we are somewhere in the middle of the list
            else
            {
                //If there is a previous element
                if (node->previous != nullptr)
                {
                    //Set the node to the previous element
                    node = node->previous;
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
        linked_list_iterator operator--(int)
        {
            //Store the current state of the iterator
            linked_list_iterator previousState = *this;
            //If the node is nullptr, then we are at the end of the list
            if (node == nullptr)
            {
                //Get the last element in the list
                node = list->lastElement;
                //node = std::make_shared<linked_list_node>(list->cbegin());
                //If the last element is null, then the list is empty
                if (node == nullptr)
                {
                    throw std::exception("Attempting to iterate over an empty linked list");
                }
            }
            //If we are somewhere in the middle of the list
            else
            {
                //If there is a previous element
                if (node->previous != nullptr)
                {
                    //Set the node to the previous element
                    node = node->previous;
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
        T& operator*()
        {
            //Return a reference to the value
            return node->value;
        }
        //Used for dereferencing the value
        T* operator->()
        {
            //Return a pointer to the value
            return &node->value;
        }

        //Tests for equality
        bool operator==(const linked_list_iterator& rhs) const
        {
            return rhs.node == node && rhs.list == list;
        }
        //Tests for inequality
        bool operator!=(const linked_list_iterator& rhs) const
        {
            return rhs.node != node || rhs.list != list;
        }
    };
private:
    using node_ptr = std::shared_ptr<linked_list_node>;

    node_ptr firstElement = nullptr;
    node_ptr lastElement = nullptr;
    int internal_size = 0;

    //Inserts a new node. The node will be inserted before the "elementToInsertBefore" iterator
    linked_list_iterator insert(linked_list_node* newNode, const linked_list_iterator elementToInsertBefore)
    {
        //If the iterator is equal to the end() iterator
        if (elementToInsertBefore.node == nullptr)
        {
            //If there is a last element
            if (lastElement != nullptr)
            {
                //Set the last element's next ptr to the new node
                lastElement->next = newNode;
                //Set the new node's previous ptr to the last element
                newNode->previous = lastElement;
            }
            //Set the last element to be the new node
            lastElement = newNode;
            //If there is no first element
            if (firstElement == nullptr)
            {
                //Set the new node to be the first element too
                firstElement = newNode;
            }
            //Increase the size of the list
            internal_size++;
        }
        //If the iterator is not the end() iterator
        else
        {
            //If the specified element has a previous node
            if (elementToInsertBefore.node->previous != nullptr)
            {
                //Get the previous node
                linked_list_node* previousNode = elementToInsertBefore.node->previous;

                //Set the previous node's next ptr to the new node
                previousNode->next = newNode;
                //Set the new node's previous ptr to the previous node
                newNode->previous = previousNode;
            }
            //Otherwise, the only node that wouldn't have a previous node is the beginning node, so that needs to be updated
            else
            {
                firstElement = newNode;
            }
            //Get the specified element node
            linked_list_node* nextNode = elementToInsertBefore.node;

            //Set the element's previous ptr to the new node
            nextNode->previous = newNode;
            //Set the new node's next ptr to the element node
            newNode->next = nextNode;
            //Increase the size of the list
            internal_size++;
        }
        //return a new iterator that points to the new node
        return linked_list_iterator(newNode, this);
    }

public:

    //The typedef for a normal iterator
    //using iterator = linked_list_iterator;
    //The typedef for an iterator that prevents mutability
    //using const_iterator = linked_list<const T>::linked_list_iterator;

    //A default constructor
    linked_list() {}

    //A copy constructor that makes a new list from an existing list
    linked_list(const linked_list<T>& toCopy)
    {
        //Add each of the copy's elements to the current list
        for (auto& val : toCopy)
        {
            push_back(val);
        }
    }

    //A move constructor that makes a new list by tranferring the contents from an old one
    linked_list(linked_list<T>&& toMove) noexcept
    {
        //Move the first element, last element, and size fields to the current list
        firstElement = toMove.firstElement;
        lastElement = toMove.lastElement;
        internal_size = toMove.internal_size;

        //Nullify/zero out the old fields
        toMove.firstElement = nullptr;
        toMove.lastElement = nullptr;
        toMove.internal_size = 0;
    }

    //A copy assignment operator that makes the current list equivalent to the "other" one
    linked_list<T>& operator=(const linked_list<T>& other)
    {
        //Clear out the existing contents
        clear();
        //Add each of the other list's elements to the current list
        for (auto& val : other)
        {
            push_back(val);
        }
    }

    //A move assignment operator that takes the contents of the "other" list and transfers it to the current one
    linked_list<T>& operator=(linked_list<T>&& other) noexcept
    {
        //Clear out the existing contents
        clear();
        //Move the first element, last element, and size fields to the current list
        firstElement = other.firstElement;
        lastElement = other.lastElement;
        internal_size = other.internal_size;

        //Nullify/zero out the old fields
        other.firstElement = nullptr;
        other.lastElement = nullptr;
        other.internal_size = 0;
    }

    //The destructor that is called when the list is done being used
    ~linked_list()
    {
        //Clear out the contents
        clear();
    }


    //Attempts to find a node with the same value as dataToFind. Returns the end() iterator if it couldn't be found
    template<typename U = T> //This template parameter is to allow both rValues and lValues
    linked_list_iterator find(U&& dataToFind)
    {
        //Start at the first node in the list
        std::shared_ptr<linked_list_node> currentNode = firstElement;

        //Loop over all the nodes in the list
        while (currentNode != nullptr)
        {
            //If a node has the same value as the dataToFind
            if (currentNode->Value == dataToFind)
            {
                //Return an iterator to that node
                return iterator(currentNode, this);
            }
            //If its not equal
            else
            {
                //Move to the next node
                currentNode = currentNode->next;
            }
        }
        //Return the end() iterator
        return end();
    }

    //Attempts to find a node with the same value as dataToFind. Returns the end() iterator if it couldn't be found
    template<typename U = T> //This template parameter is to allow both rValues and lValues
    const_iterator find(U&& dataToFind) const
    {
        //Start at the first node in the list
        std::shared_ptr<linked_list_node> currentNode = firstElement;

        //Loop over all the nodes in the list
        while (currentNode != nullptr)
        {
            //If a node has the same value as the dataToFind
            if (currentNode->Value == dataToFind)
            {
                //Return an iterator to that node
                return const_iterator(currentNode, this);
            }
            //If its not equal
            else
            {
                //Move to the next node
                currentNode = currentNode->next;
            }
        }
        //Return the end() iterator
        return cend();
    }

    //Returns an iterator to the first element in the list
    iterator begin()
    {
        return iterator(firstElement, this);
    }

    //Returns an iterator that is after the last element in the list
    iterator end()
    {
        return iterator(nullptr, this);
    }

    //Returns a const iterator to the first element in the list
    const_iterator begin() const
    {
        return cbegin();
    }

    //Returns a const iterator that is after the last element in the list
    const_iterator end() const
    {
        return cend();
    }

    //Returns a const iterator to the first element in the list
    const_iterator cbegin() const
    {
        return const_iterator(firstElement, this);
    }

    //Returns a const iterator that is after the last element in the list
    const_iterator cend() const
    {
        return const_iterator(nullptr, this);
    }

    //Adds a new element to the end of the lsit
    iterator push_back(const T& value)
    {
        //Insert at the end of the list
        return insert(value, end());
    }

    //Adds a new element to the back of the list
    iterator push_front(const T& value)
    {
        //Insert at the beginning of the list
        return insert(value, begin());
    }

    //Adds a new element to the end of the lsit
    iterator push_back(T&& value)
    {
        //Insert at the end of the list
        return insert(std::move(value), end());
    }

    //Adds a new element to the back of the list
    iterator push_front(T&& value)
    {
        //Insert at the beginning of the list
        return insert(std::move(value), begin());
    }

    //Inserts a new element before the specified position. Returns an iterator to the new node
    iterator insert(const T& value, const iterator elementToInsertBefore)
    {
        //The new node to insert
        linked_list_node* newNode = nullptr;

        try
        {
            //Allocate the node
            newNode = new linked_list_node(value);
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
    iterator insert(T&& value, const iterator elementToInsertBefore)
    {
        //The new node to insert
        linked_list_node* newNode = nullptr;

        try
        {
            //Allocate the node
            newNode = new linked_list_node(std::move(value));
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
    void delete_element(const iterator elementToRemove)
    {
        //If the elementTRemove is the same as the end() iterator
        if (elementToRemove.node == nullptr)
        {
            //Cannot delete the end() iterator, since that doesn't have a valid value
            throw std::exception("The passed in iterator does not point to a valid element");
        }
        //Get the node of the element to remove
        linked_list_node* node = elementToRemove.node;

        //If the node is the first element
        if (node == firstElement)
        {
            //Make the first element point to the node after the first element
            firstElement = node->next;
        }

        //If the node is the last element
        if (node == lastElement)
        {
            //Make the last element point to the node previous to the last
            lastElement = node->previous;
        }

        //If the node has a previous node
        if (node->previous != nullptr)
        {
            //Set the next ptr of the previous node to the one after the current node
            node->previous->next = node->next;
        }
        //If the node has a next node
        if (node->next != nullptr)
        {
            //Set the previous ptr of the next node to the one before the current node
            node->next->previous = node->previous;
        }
        //Decrease the size
        internal_size--;
        //Delete the current node
        delete node;
    }

    //Clears the list, making it empty
    void clear()
    {
        //If there is a first element, then the list contains at least 1 element in it
        if (firstElement != nullptr)
        {
            //Get the first element
            linked_list_node* currentElement = firstElement;
            //Loop over all the nodes untill the currentElement comes up null
            do
            {
                //Get the next node
                linked_list_node* nextElement = currentElement->next;
                //Delete the current node
                delete currentElement;
                //Set the current element to the next one
                currentElement = nextElement;
            } while (currentElement != nullptr);
        }

        //Nullify the first and last elements, and reset the size
        firstElement = nullptr;
        lastElement = nullptr;
        internal_size = 0;
    }

    //Returns the size of the list
    int size() const
    {
        //Return the size
        return internal_size;
    }

    //Gets the maximum value in the list. Returns end() if the list is empty
    iterator maximum()
    {
        //The max element stores the largest value. If the list is empty, it defaults to the end iterator
        iterator maxElement = end();

        //Loop over all the nodes in the list
        for (iterator i = begin(); i != end(); i++)
        {
            //If the maxElement is the end() or a larger value has been found
            if (maxElement.node == nullptr || *i > *maxElement)
            {
                //Set the maxElement iterator to the current iterator
                maxElement = i;
            }
        }

        //Return the max element
        return maxElement;
    }

    //Gets the minimum value in the list. Returns end() if the list is empty
    iterator minimum()
    {
        //The min element stores the smallest value. If the list is empty, it defaults to the end iterator
        iterator minElement = end();

        //Loop over all the nodes in the list
        for (iterator i = begin(); i != end(); i++)
        {
            //If the minElement is the end() or a smaller value has been found
            if (minElement.node == nullptr || *i < *minElement)
            {
                //Set the minElement iterator to the current iterator
                minElement = i;
            }
        }

        //Return the min element
        return minElement;
    }

    //Gets the maximum value in the list. Returns cend() if the list is empty
    const_iterator maximum() const
    {
        //The max element stores the largest value. If the list is empty, it defaults to the end iterator
        const_iterator maxElement = end();

        //Loop over all the nodes in the list
        for (const_iterator i = begin(); i != end(); i++)
        {
            //If the maxElement is the end() or a larger value has been found
            if (maxElement.node == nullptr || *i > *maxElement)
            {
                //Set the maxElement iterator to the current iterator
                maxElement = i;
            }
        }

        //Return the max element
        return maxElement;
    }

    //Gets the minimum value in the list. Returns end() if the list is empty
    const_iterator minimum() const
    {
        //The min element stores the smallest value. If the list is empty, it defaults to the end iterator
        const_iterator minElement = end();

        //Loop over all the nodes in the list
        for (const_iterator i = begin(); i != end(); i++)
        {
            //If the minElement is the end() or a smaller value has been found
            if (minElement.node == nullptr || *i < *minElement)
            {
                //Set the minElement iterator to the current iterator
                minElement = i;
            }
        }

        //Return the min element
        return minElement;
    }
};

//Used for outputting the DoublyLinkedList to the console
template<typename T>
std::ostream& operator<<(std::ostream& stream, const linked_list<T>& dt)
{
    //Print the starting bracket
    stream << "[ ";

    //If the list is empty
    if (dt.size() == 0)
    {
        //Print the ending bracket
        stream << "]";
    }
    else
    {
        //Get the last VALID element in the list
        auto lastElement = --dt.end();

        //Loop over all the elements except for the last
        for (auto i = dt.begin(); i != lastElement; i++)
        {
            //Print the value and a comma seperator
            stream << *i << ", ";
        }

        //Print the last element and the ending bracket
        stream << *lastElement << " ]";
    }

    //Return a reference to the stream
    return stream;
}*/