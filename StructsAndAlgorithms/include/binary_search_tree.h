#pragma once

#include <vector>
#include <ostream>
#include <algorithm>
#include <type_traits>
#include <common_traits.h>
#include <struct_exception.h>


//An AVL Binary Search Tree that stores a list of items in the form a tree. It's AVL, meaning, it can automatically balance itself to provide the best performance possible
template<typename T>
class binary_search_tree
{
    //Represents a node in the tree. Each node can have a parent node, and two child nodes.
    struct node
    {
        T data; //The data that this node contains
        node* parent; //The parent of this node. If the node doesn't have a parent, then this is nullptr
        node* leftChild; //The left child of the node. The left child's data is guaranteed to be less than the data of its parent. If the node doesn't have a left child, then this is nullptr
        node* rightChild; //The right child of the node. The right child's data is guaranteed to be greater than the data of its parent. If the node doesn't have a right child, then this is nullptr
        int height = 1; //The height of the node from the bottom of the tree. For example, the lowest node has a height of 1, its parent has a height of 2, it's grand-parent has a height of 3, and so on

        //Constructs a node by copying the data into the data field
        node(const T& data, node* parent, node* leftChild, node* rightChild) :
            data(data),
            parent(parent),
            leftChild(leftChild),
            rightChild(rightChild) {}

        //Constructs a node by moving the data into the data field
        node(T&& data, node* parent, node* leftChild, node* rightChild) :
            data(std::move(data)),
            parent(parent),
            leftChild(leftChild),
            rightChild(rightChild) {}
    };

    node* root = nullptr; //Represents the top most node in the tree. If this node is null, then the tree is empty.
    int treeSize = 0; //Represents how many nodes are in the tree.
    bool selfBalancing = true; //Whether the tree is self-balancing or not

    //Updates the height values of a node and recursively update the heights of its parents
    void UpdateHeights(node* node)
    {
        //If the node is null, then there is nothing that can be done
        if (node == nullptr)
        {
            return;
        }
        //If balancing is turned off, then there's no need to update the heights
        if (!selfBalancing)
        {
            return;
        }
        while (node != nullptr)
        {
            //If the node has BOTH a left and right child
            if (node->leftChild != nullptr && node->rightChild != nullptr)
            {
                //Get the old height
                int oldHeight = node->height;
                //Get the new height based on the children. We pick the child with the largest height value
                int newHeight = (node->leftChild->height > node->rightChild->height ? node->leftChild->height : node->rightChild->height) + 1;
                //If the height has changed
                if (newHeight != oldHeight)
                {
                    //Set the new height and go up to the parent and check it's height
                    node->height = newHeight;
                    node = node->parent;
                    // UpdateHeights(node->parent);
                }
                else
                {
                    break;
                }
            }
            //If the node has NEITHER a left and right child
            else if (node->leftChild == nullptr && node->rightChild == nullptr)
            {
                //Get the old height
                int oldHeight = node->height;
                //If the node has no children, then it automatically has a height of 1
                int newHeight = 1;
                //If the new height is different
                if (newHeight != oldHeight)
                {
                    //Update the height and update the height of the parent
                    node->height = newHeight;
                    node = node->parent;
                    //UpdateHeights(node->parent);
                }
                else
                {
                    break;
                }
            }
            //If the node has ONLY a left child
            else if (node->leftChild != nullptr)
            {
                //Get the old height
                int oldHeight = node->height;
                //Get the height of the left child plus 1
                int newHeight = node->leftChild->height + 1;
                //If the height has changed
                if (newHeight != oldHeight)
                {
                    //Update the height and update the height of the parent
                    node->height = newHeight;
                    node = node->parent;
                    //UpdateHeights(node->parent);
                }
                else
                {
                    break;
                }
            }
            //If the node has ONLY a right child
            else if (node->rightChild != nullptr)
            {
                //Get the old height
                int oldHeight = node->height;
                //Get the height of the right child plus 1
                int newHeight = node->rightChild->height + 1;
                //If the height has changed
                if (newHeight != oldHeight)
                {
                    //Update the height and update the height of the parent
                    node->height = newHeight;
                    node = node->parent;
                    //UpdateHeights(node->parent);
                }
                else
                {
                    break;
                }
            }
        }
    }

    //Inserts a new value into the tree as a child of the parent node. The template parameter is to allow passing an lvalue and rvalue into the data parameter
    template<typename DataType>
    node* insert(DataType&& data, node* parent)
    {
        while (parent != nullptr)
        {
            //If the data to be inserted is less than the parent data, then the new node should be a left child
            if (data < parent->data)
            {
                //If the parent doesn't have a left child
                if (parent->leftChild == nullptr)
                {
                    //Increase the tree size
                    treeSize++;
                    //Create the new node
                    node* newNode = new node(std::forward<DataType>(data), parent, nullptr, nullptr);
                    //Set the new node to be a left child of the parent
                    parent->leftChild = newNode;
                    //Update the height value of the parent. This is only run when selfBalancing is enabled
                    UpdateHeights(parent);
                    //Rebalance the tree if necessary. This is only run when selfBalancing is enabled
                    balance(newNode);
                    //Return the new node
                    return newNode;
                }
                //If the parent has a left child, then go to it and try to insert it there
                else
                {
                    parent = parent->leftChild;
                }
            }
            //If the data to be inserted is greater than the parent data, then the new node should be a right child
            else if (data > parent->data)
            {
                //If the parent doesn't have a right child
                if (parent->rightChild == nullptr)
                {
                    //Increase the tree size
                    treeSize++;
                    //Create the new node
                    node* newNode = new node(std::forward<DataType>(data), parent, nullptr, nullptr);
                    //Set the new node to be a right child of the parent
                    parent->rightChild = newNode;
                    //Update the height value of the parent. This is only run when selfBalancing is enabled
                    UpdateHeights(parent);
                    //Rebalance the tree if necessary. This is only run when selfBalancing is enabled
                    balance(newNode);
                    //Return the new node
                    return newNode;
                }
                //If the parent has a right child, then go to it and try to insert it there
                else
                {
                    parent = parent->rightChild;
                }
            }
            //If the data is equal to the parent, then we can't insert or else the tree structure is damaged
            else
            {
                return nullptr;
            }
        }
        return nullptr;
    }

    //Finds a value in a subtree. The template parameter is to allow passing an lvalue and rvalue into the dataToFind parameter
    template<typename DataType>
    node* find(DataType&& dataToFind, node* subtree)
    {
        while (subtree != nullptr)
        {
            //If the data of the subtree is equal to the data we are looking for, then return the subtree node
            if (subtree->data == dataToFind)
            {
                return subtree;
            }
            //If the data is less than the subtree node, then try to find the data in the left child subtree
            if (dataToFind < subtree->data)
            {
                subtree = subtree->leftChild;
                //return find(std::forward<DataType>(dataToFind), subtree->leftChild);
            }
            //If the data is greater than the subtree node, then try to find the data in the right child subtree
            else
            {
                subtree = subtree->rightChild;
                //return find(std::forward<DataType>(dataToFind), subtree->rightChild);
            }
        }
        return nullptr;
    }

    //Finds a value in a subtree and returns a const pointer to the node. The template parameter is to allow passing an lvalue and rvalue into the dataToFind parameter
    template<typename DataType>
    const node* find(DataType&& dataToFind, const node* subtree) const
    {
        //If the subtree is null, then return null
        if (subtree == nullptr)
        {
            return nullptr;
        }
        //If the data of the subtree is equal to the data we are looking for, then return the subtree node
        if (subtree->data == dataToFind)
        {
            return subtree;
        }
        //If the data is less than the subtree node, then try to find the data in the left child subtree
        if (dataToFind < subtree->data)
        {
            return find(std::forward<DataType>(dataToFind), subtree->leftChild);
        }
        //If the data is greater than the subtree node, then try to find the data in the right child subtree
        else
        {
            return find(std::forward<DataType>(dataToFind), subtree->rightChild);
        }
    }

    //Deletes a node from the tree. Returns true if a node has been deleted
    bool remove(node* node)
    {
        //If the node is null, then there is nothing we can do here
        if (node == nullptr)
        {
            return false;
        }
        //If the node we are deleting has BOTH a left and right child
        if (node->leftChild != nullptr && node->rightChild != nullptr)
        {
            //Find the smallest value in the right child's subtree
            auto mininumNode = minimum(node->rightChild);
            //Swap the values of the smallest value and the node we are deleting
            std::swap(node->data, mininumNode->data);
            //Delete the smallest value node
            return remove(mininumNode);
        }
        //If the node we are deleting has NEITHER a left and right child
        else if (node->leftChild == nullptr && node->rightChild == nullptr)
        {
            //If the node has a parent
            if (node->parent != nullptr)
            {
                //If the node we are deleting is the parent's right child
                if (node->parent->rightChild == node)
                {
                    //Set the parent's right child pointer to null
                    node->parent->rightChild = nullptr;
                }
                //If the node we are deleting is the parent's left child
                else if (node->parent->leftChild == node)
                {
                    //Set the parent's left child pointer to null
                    node->parent->leftChild = nullptr;
                }
                //Update the height value of the parent
                UpdateHeights(node->parent);
                //Balance the tree if necessary
                balance(node->parent);
            }
            //If the node we are deleting is the root node, then set the root to be nullptr
            if (node == root)
            {
                root = nullptr;
            }
            //Delete the node
            delete node;
            //Decrease the tree size
            treeSize--;
            return true;
        }
        //If the node we are deleting has EITHER a left or right child
        else
        {
            //Select the left child
            auto* child = node->leftChild;
            //If the right child exists, that means that the left child doesn't exist, so select the right child instead
            if (node->rightChild != nullptr)
            {
                child = node->rightChild;
            }

            //Set the parent of the child to be the parent of the node we are deleting
            child->parent = node->parent;
            //If the node we are deleting has a parent
            if (node->parent != nullptr)
            {
                //If the node we are deleting is the right child of the parent
                if (node->parent->rightChild == node)
                {
                    //Set the right child of the parent to the child of the node we are deleting
                    node->parent->rightChild = child;
                }
                //If the node we are deleting is the left child of the parent
                else if (node->parent->leftChild == node)
                {
                    //Set the left child of the parent to the child of the node we are deleting
                    node->parent->leftChild = child;
                }
                //Update the height of the parent
                UpdateHeights(node->parent);
                //Rebalance the tree if necessary
                balance(node->parent);
            }
            //If the node we are deleting is the root
            if (node == root)
            {
                //Set the root to be the child we selected
                root = child;
            }
            //Delete the node
            delete node;
            //Decrease the tree size
            treeSize--;
            return true;
        }
    }

    //Returns the largest value in the subtree
    node* maximum(node* subTree)
    {
        while (subTree != nullptr)
        {
            //If the subtree has a right child, then find the maximum in that subtree
            if (subTree->rightChild != nullptr)
            {
                subTree = subTree->rightChild;
            }
            //If the subtree does not have a right child, then we found the largest value
            else
            {
                return subTree;
            }
        }
        return nullptr;
    }

    //Returns the smallest value in the subtree
    node* minimum(node* subTree)
    {
        while (subTree != nullptr)
        {
            //If the subtree has a left child, then find the minimum in that subtree
            if (subTree->leftChild != nullptr)
            {
                subTree = subTree->leftChild;
            }
            //If the subtree does not have a left child, then we found the smallest value
            else
            {
                return subTree;
            }
        }
        return nullptr;
    }

    //Returns a const pointer to the largest value in the subtree
    const node* maximum(const node* subTree) const
    {
        while (subTree != nullptr)
        {
            //If the subtree has a right child, then find the maximum in that subtree
            if (subTree->rightChild != nullptr)
            {
                subTree = subTree->rightChild;
            }
            //If the subtree does not have a right child, then we found the largest value
            else
            {
                return subTree;
            }
        }
        return nullptr;
    }

    //Returns a const pointer to the smallest value in the subtree
    const node* minimum(const node* subTree) const
    {
        while (subTree != nullptr)
        {
            //If the subtree has a left child, then find the minimum in that subtree
            if (subTree->leftChild != nullptr)
            {
                subTree = subTree->leftChild;
            }
            //If the subtree does not have a left child, then we found the smallest value
            else
            {
                return subTree;
            }
        }
        return nullptr;
    }

    //Returns how balanced a subtree is. A value less than -1 means it is right-heavy, and a value greater than 1 means it is left-heavy
    int getSubtreeBalance(const node* subTree) const
    {
        //If the subtree is null, then there is nothing we can do
        if (subTree == nullptr)
        {
            return 0;
        }
        //The height of the left subtree
        int leftHeight = 0;
        //If the subtree has a left child
        if (subTree->leftChild != nullptr)
        {
            //Get the height of the left subtree
            leftHeight = subTree->leftChild->height;
        }
        //The height of the right subtree
        int rightHeight = 0;
        //If the subtree has a right child
        if (subTree->rightChild != nullptr)
        {
            //Get the height of the right subtree
            rightHeight = subTree->rightChild->height;
        }
        //Return the difference between the two heights
        return leftHeight - rightHeight;
    }

    //Rotates a series of nodes to the left. Used for rebalancing.
    //Visual Examples for how rotations work can be found here : https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
    //Below is how the left rotation works visually:
    /*
         y                               x
        / \                             / \
       x   T3                          T1  y
      / \       < - - - - - - -           / \
     T1  T2     Left Rotation            T2  T3


    */
    //The variables are named according to the diagram
    void leftRotation(node* x)
    {
        //If the node does not have a right child, then rotating here is not possible
        if (x->rightChild == nullptr)
        {
            return;
        }
        //Get the right child and store it in "y"
        node* y = x->rightChild;

        //Set the right child of "x" to the left child of "y"
        x->rightChild = y->leftChild;
        //If said right child exists in the first place
        if (x->rightChild != nullptr)
        {
            //Set it's parent to "x"
            x->rightChild->parent = x;
        }

        //Set the parent of "y" to whatever the parent was of "x"
        y->parent = x->parent;
        //If "x" had a parent in the first place
        if (x->parent != nullptr)
        {
            //If the parent's left child was the node to rotate
            if (x->parent->leftChild == x)
            {
                //Set the parent's left child to be "y"
                x->parent->leftChild = y;
            }
            //If the parent's right child was the node to rotate
            else if (x->parent->rightChild == x)
            {
                //Set the parent's right child to be "y"
                x->parent->rightChild = y;
            }
        }
        //Set the parent of "x" to be "y"
        x->parent = y;

        //Set the left child of "y" to be "x"
        y->leftChild = x;
        //If x was the root, then set it to be y
        if (x == root)
        {
            root = y;
        }
        //Update the height of x
        UpdateHeights(x);

        //Update the height of y
        UpdateHeights(y);
    }

    //Rotates a series of nodes to the right. Used for rebalancing.
    //Visual Examples for how rotations work can be found here : https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
    //Below is how the right rotation works visually:
    /*
         y                               x
        / \                             / \
       x   T3                          T1  y
      / \       - - - - - - - >           / \
     T1  T2     Right Rotation           T2  T3


    */
    //The variables are named according to the diagram
    void rightRotation(node* y)
    {
        //If the node does not have a left child, then rotating here is not possible
        if (y->leftChild == nullptr)
        {
            return;
        }
        //Get the left child and store it in "x"
        node* x = y->leftChild;

        //Set the left child of "y" to the right child of "x"
        y->leftChild = x->rightChild;
        //If said left child even exists
        if (y->leftChild != nullptr)
        {
            //Update it's parent to be "y"
            y->leftChild->parent = y;
        }

        //Set the parent of x to be the parent of y
        x->parent = y->parent;

        //If the parent of y even exists
        if (y->parent != nullptr)
        {
            //If y was the left child of the parent
            if (y->parent->leftChild == y)
            {
                //Set the parent's left child to be x
                y->parent->leftChild = x;
            }
            //If y was the right child of the parent
            else if (y->parent->rightChild == y)
            {
                //Set the parent's right child to be x
                y->parent->rightChild = x;
            }
        }
        //Set the parent of y to be x
        y->parent = x;

        //Set the right child of x to be y
        x->rightChild = y;

        //If y was the root node, then x becomes the new root
        if (y == root)
        {
            root = x;
        }

        //Update the height of y
        UpdateHeights(y);

        //Update the height of x
        UpdateHeights(x);
    }

    //Used to balance a tree by applying node rotations when necessary
    void balance(node* x)
    {
        //If self-balancing is disabled, then there is nothing to do
        if (!selfBalancing)
        {
            return;
        }
        //If the tree is empty, then no balancing is needed
        if (root == nullptr)
        {
            return;
        }

        //Start at the root node and work our way down to node "x"
        node* currentNode = root;

        //Loop until we reach node "x" or until there are no more nodes left
        while (currentNode != x && currentNode != nullptr)
        {
            //Get how balance the current node is
            int balance = getSubtreeBalance(currentNode);
            //If the node is left-heavy, the node has a left child and the "x" node is a left child within the left subtree
            if (balance > 1 && currentNode->leftChild != nullptr && x->data < currentNode->leftChild->data)
            {
                //Do a right rotation
                rightRotation(currentNode);
            }
            //If the node is right-heavy, the node has a right child and the "x" node is a right child within the left subtree
            else if (balance < -1 && currentNode->rightChild != nullptr && x->data > currentNode->rightChild->data)
            {
                //Do a right rotation
                leftRotation(currentNode);
            }
            //If the node is left-heavy, the node has a left child and the "x" node is a right child within the left subtree
            else if (balance > 1 && currentNode->leftChild != nullptr && x->data > currentNode->leftChild->data)
            {
                //Do a left rotation on the left child
                leftRotation(currentNode->leftChild);
                //Do a right rotation on the current node
                rightRotation(currentNode);
            }
            //If the node is right-heavy, the node has a right child and the "x" node is a left child within the left subtree
            else if (balance < -1 && currentNode->rightChild != nullptr && x->data < currentNode->rightChild->data)
            {
                //Do a right rotation on the right child
                rightRotation(currentNode->rightChild);
                //Do a left rotation on the current node
                leftRotation(currentNode);
            }
            //If the "x" node is somewhere in the left subtree
            if (x->data < currentNode->data)
            {
                //Go to the left child
                currentNode = currentNode->leftChild;
            }
            //If the "x" node is somewhere in the right subtree
            else
            {
                //Go to the right child
                currentNode = currentNode->rightChild;
            }
        }

    }



    /*An iterator for accessing nodes within the tree and iterating through them
      This iterator can only go in the forward direction: http://www.cplusplus.com/reference/iterator/ForwardIterator/
      The is_const flag is to determine if the fields of this class will be const or not
      This is done this way because of two reasons:
        1. The const_iterator should still allow the user to iterate over them
        2. Doing it this way allows the constructor parameters to also be const, allowing const_iterators to be created in const functions in the binary_search_tree class
    */
    //This new version can be easily carried over to future projects
    template<bool is_const>
    class iterator_base
    {
        //Used for accessing the private details of the binary_search_tree class
        friend class binary_search_tree<T>;

        //The type of node this iterator will be accessing. This type will be const if "is_const" is true
        using NodeType = make_const_if_true<node, is_const>;
        //The type of tree the iterator will be accessing. This type will be const if "is_const" is true
        using TreeType = make_const_if_true<binary_search_tree<T>, is_const>;

        //The node that the iterator points to. This will be "const node*" if "is_const" is true
        NodeType* nodePtr;

        //Previous node that the iterator was previously. This will be "const node*" if "is_const" is true
        NodeType* previousNode = nullptr;

        //The binary search tree the iterator is a part of. This will be "const binary_search_tree<T>*" if "is_const" is true
        TreeType* tree;

        //Constructs a new iterator from a node and tree
        iterator_base(NodeType* node, TreeType* tree) : nodePtr(node), tree(tree) { }

    public:
        //An implicit copy constructor for implicity converting non-const iterators to const versions
        iterator_base(const iterator_base<false>& other) : iterator_base(other.nodePtr, other.tree) {}
        //An implicit move constructor for implicity converting non-const iterators to const versions
        iterator_base(iterator_base<false>&& other) noexcept : iterator_base(other.nodePtr, other.tree)
        {
            other.nodePtr = nullptr;
            other.previousNode = nullptr;
            other.tree = nullptr;
        }

        //These type definitions are required for iterators
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = int;

        //Pre-increments the iterator to the next value
        //This is used to iterate over the tree using the in-order traversal method, which has been improvised to support iterators
        iterator_base<is_const>& operator++()
        {

            //If the node is null, then we are already at the end
            if (nodePtr == nullptr)
            {
                throw struct_exception("Cannot iterate past the end of the tree");
            }

            //If the node doesn't have a defined previous location. PreviousNode is null only when we first start iterating through the tree
            if (previousNode == nullptr)
            {
                //If the node has a rightmost child, then move down to the lowest value in that subtree
                if (nodePtr->rightChild != nullptr)
                {
                    nodePtr = tree->minimum(nodePtr->rightChild);
                    previousNode = nullptr;
                }
                //Otherwise, if the node has a parent, then jump to that
                else if (nodePtr->parent != nullptr)
                {
                    previousNode = nodePtr;
                    nodePtr = nodePtr->parent;
                    //If we came from a right child, then keep going up until we reach a parent from the left side
                    while (previousNode == nodePtr->rightChild)
                    {
                        //If the node does not have a parent, then we have reached the end
                        if (nodePtr->parent == nullptr)
                        {
                            nodePtr = nullptr;
                            previousNode = nullptr;
                            break;
                        }
                        //If the node does have a parent, then keep going up the subtree
                        else
                        {
                            previousNode = nodePtr;
                            nodePtr = nodePtr->parent;
                        }
                    }
                }
                //If the node does not have a parent, then we have reached the end, since the only node that cannot have a parent is root
                else
                {
                    //We have reached the end of the tree
                    nodePtr = nullptr;
                    previousNode = nullptr;
                }
            }
            //If we do have a previous location, then we can use that to determine what to do next
            else
            {
                //If we came from a left child
                if (previousNode == nodePtr->leftChild)
                {
                    //If the node we are on right now has a right child, then we can start traversing down it's subtree
                    if (nodePtr->rightChild != nullptr)
                    {
                        //Find the lowest value in the subtree and start from there
                        nodePtr = tree->minimum(nodePtr->rightChild);
                        previousNode = nullptr;
                    }
                    //If the node does not have a right child, then go up to it's parent
                    else
                    {
                        //If there is no parent, then we are at the end, since the only node that cannot have a parent is root
                        if (nodePtr->parent == nullptr)
                        {
                            previousNode = nullptr;
                            nodePtr = nullptr;
                        }
                        //If there is a parent, then go to it
                        else
                        {
                            previousNode = nodePtr;
                            nodePtr = nodePtr->parent;
                            //If we came from a right child, then keep going up until we reach a parent from the left side
                            while (previousNode == nodePtr->rightChild)
                            {
                                //If the node does not have a parent, then we have reached the end
                                if (nodePtr->parent == nullptr)
                                {
                                    nodePtr = nullptr;
                                    previousNode = nullptr;
                                    break;
                                }
                                //If the node does have a parent, then keep going up the subtree
                                else
                                {
                                    previousNode = nodePtr;
                                    nodePtr = nodePtr->parent;
                                }
                            }
                        }
                    }
                }
            }

            return *this;
        }
        //Post-increments the iterator to the next value
        iterator_base<is_const> operator++(int)
        {
            //Store the previous state of the iterator
            iterator_base<is_const> previous = *this;
            //Increment the iterator
            this->operator++();
            //Return the previous state
            return previous;

        }

        //Used to get the value of the iterator
        //This needs to be const because modifying the value in the tree would mess with the tree's structure
        const T& operator*() const
        {
            //Return a reference to the value
            return nodePtr->data;
        }
        //Used for dereferencing the value
        //This needs to be const because modifying the value in the tree would mess with the tree's structure
        const T* operator->() const
        {
            //Return a pointer to the value
            return &nodePtr->data;
        }

        //Tests for inequality
        bool operator!=(const iterator_base<is_const>& rhs) const
        {
            return rhs.nodePtr != nodePtr || rhs.tree != tree;
        }

        //Tests for equality
        bool operator==(const iterator_base<is_const>& rhs) const
        {
            return rhs.nodePtr == nodePtr && rhs.tree == tree;
        }

        //A copy assignment operator used for assigning a non-const iterator to a const version
        iterator_base<is_const>& operator=(const iterator_base<false>& other) noexcept
        {
            nodePtr = other.nodePtr;
            previousNode = other.previousNode;
            tree = other.tree;
            return *this;
        }

        //A move assignment operator used for assigning a non-const iterator to a const version
        iterator_base<is_const>& operator=(iterator_base<false>&& other) noexcept
        {
            nodePtr = other.nodePtr;
            previousNode = other.previousNode;
            tree = other.tree;

            other.nodePtr = nullptr;
            other.previousNode = nullptr;
            other.tree = nullptr;
            return *this;
        }
    };

public:
    //A non_const version of the iterator
    using iterator = iterator_base<false>;
    //A const version of the iterator, where all the fields and constructor parameters are const
    using const_iterator = iterator_base<true>;

    //Clears the tree
    void clear()
    {
        if (root != nullptr)
        {
            for (auto i = begin(); i != end();)
            {
                auto next = i;
                next++;
                remove(i);
                i = next;
            }
            treeSize = 0;
            root = nullptr;
        }
    }

    //Default constructor for a binary search tree
    binary_search_tree() {}

    //Constructs a new binary search tree from an intializer list
    binary_search_tree(const std::initializer_list<T> list)
    {
        //Loop over all the values in the list and add them into the tree
        for (const auto& value : list)
        {
            insert(value);
        }
    }

    //A copy constructor for creating a new tree from a copy
    binary_search_tree(const binary_search_tree<T>& toCopy)
    {
        //Loop over all the values in the copy
        for (auto& value : toCopy)
        {
            //Insert them into the new tree
            insert(value);
        }
    }

    //A move constructor for creating a new tree by moving the data from an old tree
    binary_search_tree(binary_search_tree<T>&& toMove) noexcept
    {
        //Move the root node
        root = toMove.root;
        //Move the tree size
        treeSize = toMove.treeSize;
        //Reset the root and treeSize of the old tree
        toMove.root = nullptr;
        toMove.treeSize = 0;
    }

    //A copy assignment operator for making a tree identical to a copy
    binary_search_tree<T>& operator=(const binary_search_tree<T>& toCopy) noexcept
    {
        //Clear the current tree
        clear();
        //Loop over all the values in the copy
        for (auto& value : toCopy)
        {
            //Insert each of the values into the current tree
            insert(value);
        }
        //Return the current tree
        return *this;
    }

    //A move assignment operator for taking the contents of an existing tree and moving them to the current tree
    binary_search_tree<T>& operator=(binary_search_tree<T>&& toMove) noexcept
    {
        //Clear the existing tree
        clear();
        //Move the root and tree size values to the current tree
        root = toMove.root;
        treeSize = toMove.treeSize;
        //Reset the root and tree size value of the old tree
        toMove.root = nullptr;
        toMove.treeSize = 0;
        //Return the current tree
        return *this;
    }

    //The deconstructor for the tree
    ~binary_search_tree() noexcept
    {
        //Clear the tree
        clear();
    }

    //Inserts a new element into the tree. Returns end() if the element cannot be inserted. Duplicates are not allowed
    //The template parameter is to allow the function to take both rvalues and lvalues
    template<typename DataType>
    iterator insert(DataType&& data)
    {
        //If the root is null, then the new node becomes the root
        if (root == nullptr)
        {
            //Increase the tree size
            treeSize++;
            //Create the new node as the root
            root = new node(std::forward<DataType>(data), nullptr, nullptr, nullptr);
            //Return an iterator to the root
            return iterator(root, this);
        }
        //If the root is not null, then traverse the tree to find a spot for the value
        else
        {
            //Insert into the tree and return an iterator to it
            return iterator(insert(std::forward<DataType>(data), root), this);
        }
    }

    //Deletes a node from the tree. Returns true if a node has been deleted
    //The template parameter is to allow the function to take both rvalues and lvalues.
    template<typename DataType = T>
    bool remove(DataType&& data)
    {
        //First, find the data in the tree. Then, delete that node from the tree
        return remove(find(std::forward<DataType>(data), root));
    }

    //Deletes a node from the tree. Returns true if a node has been deleted
    bool remove(iterator elementToDelete)
    {
        //Get the node that the iterator points to a delete it
        return remove(elementToDelete.nodePtr);
    }

    //Attempts to find data in the tree and returns an iterator to that data. If the data could not be found, then the end() iterator is returned
    template<typename DataType>
    iterator find(DataType&& data)
    {
        //Find the data in the tree and return an iterator to it
        return iterator(find(std::forward<DataType>(data), root), this);
    }

    //Attempts to find data in the tree and returns an iterator to that data. If the data could not be found, then the end() iterator is returned
    template<typename DataType>
    const_iterator find(DataType&& data) const
    {
        //Find the data in the tree and return an iterator to it
        return const_iterator(find(std::forward<DataType>(data), root), this);
    }

    //Returns the largest value in the tree. Returns end() if the tree is empty
    iterator maximum()
    {
        //Return the largest value in the tree, starting from root
        return iterator(maximum(root), this);
    }

    //Returns the smallest value in the tree. Returns end() if the tree is empty
    iterator minimum()
    {
        //Return the smallest value in the tree, starting from root
        return iterator(minimum(root), this);
    }

    //Returns the largest value in the tree. Returns end() if the tree is empty
    const_iterator maximum() const
    {
        //Return the largest value in the tree, starting from root
        return const_iterator(maximum(root), this);
    }

    //Returns the smallest value in the tree. Returns end() if the tree is empty
    const_iterator minimum() const
    {
        //Return the smallest value in the tree, starting from root
        return const_iterator(minimum(root), this);
    }

    //Get the beginning iterator. The beginning iterator is the smallest value in the tree. Incrementing the iterator will iterate over the tree in sorted order
    iterator begin()
    {
        //Return the smallest value in the tree
        return minimum();
    }

    //Get the beginning iterator. The beginning iterator is the smallest value in the tree. Incrementing the iterator will iterate over the tree in sorted order
    const_iterator cbegin() const
    {
        //Return the smallest value in the tree
        return minimum();
    }

    //Get the beginning iterator. The beginning iterator is the smallest value in the tree. Incrementing the iterator will iterate over the tree in sorted order
    const_iterator begin() const
    {
        //Return the smallest value in the tree
        return cbegin();
    }

    //Gets the ending iterator. This is the iterator that is used when finished iterating the tree
    iterator end()
    {
        return iterator(nullptr, this);
    }

    //Gets the ending iterator. This is the iterator that is used when finished iterating the tree
    const_iterator cend() const
    {
        return const_iterator(nullptr, this);
    }

    //Gets the ending iterator. This is the iterator that is used when finished iterating the tree
    const_iterator end() const
    {
        return const_iterator(nullptr, this);
    }

    //Gets the iterator to the root node, or end() if the tree is empty
    iterator getRoot()
    {
        return iterator(root, this);
    }

    //Gets the iterator to the root node, or end() if the tree is empty
    const_iterator getRoot() const
    {
        return const_iterator(root, this);
    }

    //Sets whether the tree is self balancing or not. By default, it is turned on
    void setSelfBalancing(bool value)
    {
        //If the self balancing is being enabled
        if (selfBalancing != value && value == true)
        {
            //Update the field value
            selfBalancing = value;
            //If the root is not null
            if (root != nullptr)
            {
                //Loop over all the nodes in the tree and configure their height values. The heights need to be configured property in order for the balancing to work
                for (auto i = begin(); i != end(); i++)
                {
                    UpdateHeights(i.nodePtr);
                }
            }
        }
        else
        {
            selfBalancing = value;
        }
    }

    //Returns whether the tree is self balancing or not. By default, it is turned on
    bool getSelfBalancing() const
    {
        return selfBalancing;
    }

    //Gets how many elements are in the tree
    int getSize() const
    {
        return treeSize;
    }

    //Returns a vector with all the values traversed from lowest to largest value
    std::vector<T> traverse()
    {
        //The list of elements
        std::vector<T> elements{};

        //Loop over all the values from lowest to largest
        for (auto i = begin(); i != end(); i++)
        {
            //Add them to the list
            elements.push_back(*i);
        }
        //Return the list
        return elements;
    }

    //Returns a vector with all the values traversed from lowest to largest value
    const std::vector<T> traverse() const
    {
        std::vector<T> elements{};

        //Loop over all the values from lowest to largest
        for (auto i = begin(); i != end(); i++)
        {
            //Add them to the list
            elements.push_back(*i);
        }
        //Return the list
        return elements;
    }
};

//Used for printing the tree to a stream
template<typename T>
std::ostream& operator<<(std::ostream& stream, const binary_search_tree<T>& tree)
{
    //Get the size of the tree
    int size = tree.getSize();
    //if the tree is empty
    if (size == 0)
    {
        //Print a pair of brackets
        stream << "[]";
    }
    //If the tree size is greater than 1
    else
    {
        //Print the beginning bracket
        stream << "[";
        //Get the beginning of the tree
        auto beginning = tree.begin();

        //Loop over all the values except the last in the tree
        for (int i = 0; i < (size - 1); i++)
        {
            //Print them to the stream
            stream << *beginning << ", ";

            //Increment the beginning iterator
            ++beginning;
        }

        //Print the last value of the tree along with a closing bracket
        stream << *beginning << "]";
    }
    //Return a reference to the stream
    return stream;
}

