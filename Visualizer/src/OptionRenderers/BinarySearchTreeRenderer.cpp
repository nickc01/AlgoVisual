#include "OptionRenderers/OptionRenderer.h" // Required header
#include "binary_search_tree.h" // Header for the Binary Search Tree class
#include "imgui.h" // Graphics library
#include "raylib.h" // Graphics library
#include "raymath.h" // Graphics library
#include "visual_container.h" // Header for the visual_container class
#include <OptionRenderers/BinarySearchTreeRenderer.h> // Header for the Binary Search Tree Renderer class
#include <global.h> // Global header file

namespace {
    std::string _name = "Binary Search Tree"; // Namespace with the name of the algorithm
}

// Constructor for the Binary Search Tree Renderer class
BinarySearchTreeRenderer::BinarySearchTreeRenderer() : OptionRenderer() {
    createStarterList(); // Calls function to create a starting list
}

// Function to update the renderer every frame
void BinarySearchTreeRenderer::update(double dt) {
    OptionRenderer::update(dt); // Calls base class update function

    updateNodes(tree.getRoot(), dt); // Calls function to update the nodes in the tree
}

// Function to update the nodes of the tree
void BinarySearchTreeRenderer::updateNodes(decltype(tree)::iterator node, double dt) {
    if (node == tree.end()) { // Base case: end of the tree has been reached
        return;
    }

    // Updates the position of the node based on its target position
    node->x = Lerp(node->x, node->targetX, INTERPOLATION_SPEED * dt);
    node->y = Lerp(node->y, node->targetY, INTERPOLATION_SPEED * dt);

    // Recursively updates the left and right child nodes
    updateNodes(node.getLeft(), dt);
    updateNodes(node.getRight(), dt);
}

// This function renders the binary search tree to the screen
void BinarySearchTreeRenderer::render() {
    DrawBackground(2.0); // Draw a background with a given color
    renderNodeLines(tree.getRoot()); // Render the lines connecting the nodes
    renderNode(tree.getRoot()); // Render the nodes themselves
    ImGui::Spacing(); // Add some spacing between UI elements
    ImGui::TextWrapped("A binary search tree is simply a collection of nodes. Each node can only have up to two children, a left child and a right child. The left child will always have a number value less than it's parent, while the right child will always have a number greater than its parent. This structure allows for very fast searching. No duplicate values are allowed in a tree"); // Display information about binary search trees
    ImGui::Spacing(); // Add some more spacing
    ImGui::InputFloat("Number",&selectedNumber); // Display an input field for a number to be added or removed

    if (ImGui::Button("Add Number")) // Add a button to add a number
    {
        add_number(selectedNumber); // Call a function to add the inputted number
    }

    if (ImGui::Button("Remove Number")) // Add a button to remove a number
    {
        remove_number(selectedNumber); // Call a function to remove the inputted number
    }

    if (ImGui::Button("Add Random Number")) // Add a button to add a random number
    {
        add_random_number(); // Call a function to add a random number
    }

    if (selfBalancing) { // If self-balancing is enabled
        if (ImGui::Button("Disable Self Balancing")) { // Add a button to disable self-balancing
            selfBalancing = false; // Set selfBalancing to false
            tree.setSelfBalancing(false); // Set the tree's self-balancing flag to false
        }
    }
    else { // If self-balancing is disabled
        if (ImGui::Button("Enable Self Balancing")) { // Add a button to enable self-balancing
            selfBalancing = true; // Set selfBalancing to true
            tree.setSelfBalancing(true); // Set the tree's self-balancing flag to true
        }
    }

    if (ImGui::Button("Clear")) { // Add a button to clear the tree
        clear(); // Call a function to clear the tree
    }
}

// This function renders the lines connecting the nodes in the binary search tree
void BinarySearchTreeRenderer::renderNodeLines(decltype(tree)::iterator node) {
    if (node == tree.end()) { // If the node is the end node, stop recursion
        return;
    }
    auto nodePos = transformPosition(node->x, node->y); // Get the position of the current node

    if (node.getParent() != tree.end()) { // If the current node has a parent
        auto parent = node.getParent(); // Get the parent node
        auto parentPos = transformPosition(parent->x, parent->y); // Get the position of the parent node
        DrawLineEx(nodePos, parentPos, 20 / scale, ORANGE); // Draw a line between the current node and its parent
    }

    renderNodeLines(node.getLeft()); // Recursively call this function for the left child
    renderNodeLines(node.getRight()); // Recursively call this function for the right child
}

// This function is used to render a single node of a binary search tree.
// It takes an iterator to the node as an argument.
void BinarySearchTreeRenderer::renderNode(decltype(tree)::iterator node) {
    if (node == tree.end()) {
        return;
    }

    // Calculate the position of the circle representing the node
    auto circlePos = transformPosition(node->x, node->y);

    // Draw the circle
    // DrawCircle(circlePos.x,circlePos.y,CIRCLE_SIZE / scale,RED);
    DrawTexture(getOrangeCircle(), circlePos, scale);

    // Convert the node's value to a string
    auto str = std::to_string(node->value);

    // Calculate the position of the text representing the node's value
    float textWidth = MeasureText(str.c_str(), TEXT_SIZE);
    auto textPos = transformPosition(node->x - (textWidth / 2), node->y - (TEXT_SIZE / 2));

    // Draw the text
    DrawTextEx(GetFontDefault(), str.c_str(), textPos, TEXT_SIZE / scale, (TEXT_SIZE / scale) / 10.0, WHITE);

    // Render the left and right child nodes recursively
    renderNode(node.getLeft());
    renderNode(node.getRight());
}

// Returns the name of the binary search tree algorithm being rendered
const std::string& BinarySearchTreeRenderer::getName() const {
    return _name;
}

// Generates a starter binary search tree for visualization purposes
void BinarySearchTreeRenderer::createStarterList() {
    tree.insert(visual_container<float>(5));
    tree.insert(visual_container<float>(1));
    tree.insert(visual_container<float>(2));
    tree.insert(visual_container<float>(11));
    tree.insert(visual_container<float>(15));
    tree.insert(visual_container<float>(0));
    tree.insert(visual_container<float>(4));
    tree.insert(visual_container<float>(3));
    tree.insert(visual_container<float>(9));
    tree.insert(visual_container<float>(20));
    tree.insert(visual_container<float>(13));

    // Update the positions of the nodes in the tree
    updateNodePositions(tree.getRoot());
}

// Returns the maximum value between two integers
constexpr inline int Max(int a, int b) {
    return a > b ? a : b;
}

// Sets the position of the node and its children
void BinarySearchTreeRenderer::setNodeAndChildrenPos(decltype(tree)::iterator node, float newX, float newY) {
    // If the iterator is invalid, return
    if (node == tree.end()) {
        return;
    }
    // Calculate the difference between the new position and the target position
    int diffX = newX - node->targetX;
    int diffY = newY - node->targetY;
    // Move the node and its children by the calculated amount
    moveNodeAndChildren(node, diffX, diffY);
}

// Moves the node and its children by the given amount
void BinarySearchTreeRenderer::moveNodeAndChildren(decltype(tree)::iterator node, float diffX, float diffY) {
    // If the iterator is invalid, return
    if (node == tree.end()) {
        return;
    }
    // Update the target position of the node
    node->targetX += diffX;
    node->targetY += diffY;
    // Recursively move the left and right children by the same amount
    moveNodeAndChildren(node.getLeft(), diffX, diffY);
    moveNodeAndChildren(node.getRight(), diffX, diffY);
}

// Updates the positions of the nodes in the tree
BinarySearchTreeRenderer::positionResult BinarySearchTreeRenderer::updateNodePositions(decltype(tree)::iterator node) {
    // If the iterator is invalid, return
    if (node == tree.end()) {
        return BinarySearchTreeRenderer::positionResult{};
    }

    // Recursively update the positions of the left and right children
    auto leftResult = updateNodePositions(node.getLeft());
    auto rightResult = updateNodePositions(node.getRight());

    // If the node is the root node, set its position to (0, 0)
    if (node.getParent() == tree.end()) {
        node->targetX = 0;
        node->targetY = 0;
    }
    // If the node has no children, return (0, 0)
    else if (node.getLeft() == tree.end() && node.getRight() == tree.end()) {
        return positionResult{0,0};
    }
    // Set the position of the left and right children based on the position of the current node
    setNodeAndChildrenPos(node.getLeft(), node->targetX - leftResult.rightWidth - (CIRCLE_SPACING / 2), node->targetY + CIRCLE_SPACING);
    setNodeAndChildrenPos(node.getRight(), node->targetX + rightResult.leftWidth + (CIRCLE_SPACING / 2), node->targetY + CIRCLE_SPACING);

    // Calculate the width of the left and right subtrees
    float leftWidth = leftResult.leftWidth;
    float rightWidth = rightResult.rightWidth;

    // If the node has a left child, add its width to the leftWidth variable
    if (node.getLeft() != tree.end()) {
        leftWidth += leftResult.rightWidth + (CIRCLE_SPACING / 2);
    } 

    // If the node has a right child, add its width to the rightWidth variable
    if (node.getRight() != tree.end()) {
        rightWidth += rightResult.leftWidth + (CIRCLE_SPACING / 2);
    }

    // Return the positionResult struct containing the left and right subtree widths
    return positionResult{leftWidth,rightWidth};
}


// This function adds a new number to the binary search tree and updates the node positions
bool BinarySearchTreeRenderer::add_number(float number) {
    auto node = tree.insert(visual_container<float>(number));
    if (node != tree.end()) {
        updateNodePositions(tree.getRoot()); // Update node positions after insertion
        return true;
    }
    return false;
}

// This function removes a number from the binary search tree and updates the node positions
bool BinarySearchTreeRenderer::remove_number(float number) {
    auto result = tree.find(visual_container<float>(number));
    if (result != tree.end()) {
        tree.remove(result);
        updateNodePositions(tree.getRoot()); // Update node positions after removal
        return true;
    }
    return false;
}

// This function adds 1000 random numbers to the binary search tree
bool BinarySearchTreeRenderer::add_random_number() {
    for (int i = 0; i < 1000; i++) {
        if (add_number((rand() % 2000) - 1000)) {
            return true;
        }
    }
    return false;
}

// This function clears the binary search tree
void BinarySearchTreeRenderer::clear() {
    tree.clear();
}

// This function is called when the binary search tree is being saved to a file
void BinarySearchTreeRenderer::onSave(std::ostream& outputStream) const {
    outputStream << 'b'; // Indicate that this is a binary search tree in the output file
    onSave(tree.getRoot(),outputStream); // Call the recursive onSave function
}

void BinarySearchTreeRenderer::onSave(decltype(tree)::const_iterator root, std::ostream& outputStream) const {
    if (root == tree.end()) {
        return;
    }
    outputStream << root->value;
    outputStream << 'b';
    outputStream << root->targetX;
    outputStream << 'b';
    outputStream << root->targetY;
    outputStream << 'b';
    char type = 0;
    if (root.getLeft() != tree.end()) {
        type |= 1;
    }
    if (root.getRight() != tree.end()) {
        type |= 2;
    }
    outputStream << type;
    outputStream << 'b';

    if ((type & 1) == 1) {
        onSave(root.getLeft(),outputStream);
    }
    if ((type & 2) == 2) {
        onSave(root.getRight(),outputStream);
    }
}

void BinarySearchTreeRenderer::onLoad(std::istream& inputStream) {
    char id;
    inputStream >> id;
    if (id != 'b') {
        throw std::exception();
    }
    tree.clear();
    if (!inputStream.eof()) {
        tree.setSelfBalancing(false);
        float value, targetX, targetY;
        inputStream >> value;
        inputStream >> id;
        inputStream >> targetX;
        inputStream >> id;
        inputStream >> targetY;
        inputStream >> id;
        auto container = visual_container<float>(value,targetX,targetY);

        auto root = tree.insert(std::move(container));

        char type = 0;
        inputStream >> type;
        inputStream >> id;
        if ((type & 1) == 1) {
            onLoad(root,inputStream);
        }
        if ((type & 2) == 2) {
            onLoad(root, inputStream);
        }
    }

    tree.setSelfBalancing(true);
}

void BinarySearchTreeRenderer::onLoad(decltype(tree)::iterator node, std::istream& inputStream) {
    char id;
    float value, targetX, targetY;
    inputStream >> value;
    inputStream >> id;
    inputStream >> targetX;
    inputStream >> id;
    inputStream >> targetY;
    inputStream >> id;
    auto container = visual_container<float>(value,targetX,targetY);

    auto next = tree.insert(std::move(container),node);

    char type = 0;
    inputStream >> type;
    inputStream >> id;
    if ((type & 1) == 1) {
        onLoad(next,inputStream);
    }
    if ((type & 2) == 2) {
        onLoad(next, inputStream);
    }
}
