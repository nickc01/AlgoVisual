//This file contains the header file for the BinarySearchTreeRenderer class. Used for rendering binary search trees.

#pragma once

#include "OptionRenderers/OptionRenderer.h"
#include "binary_search_tree.h"
#include "visual_container.h"

class BinarySearchTreeRenderer : public OptionRenderer {

//The binary search tree to be rendered
binary_search_tree<visual_container<float>> tree{
    [](const visual_container<float> &a, const visual_container<float> &b) {
        return a.value < b.value;
    }};

//The result of updating node positions
struct positionResult {
    float leftWidth = 0;
    float rightWidth = 0;

    
    //Default constructor
    constexpr positionResult(){};

    //Constructor with left and right widths as parameters
    constexpr positionResult(float _l, float _r) : leftWidth(_l), rightWidth(_r) {}
  };

    //Renders a given node
    void renderNode(decltype(tree)::iterator node);

    //Renders lines connecting nodes
    void renderNodeLines(decltype(tree)::iterator node);

    //Updates node positions over time
    void updateNodes(decltype(tree)::iterator node, double dt);

    //Creates a starting list of nodes
    void createStarterList();

    //Updates node positions and returns a positionResult
    positionResult updateNodePositions(decltype(tree)::iterator node);

    //Moves a node and its children by a given amount
    void moveNodeAndChildren(decltype(tree)::iterator node, float diffX, float diffY);

    //Sets the position of a node and its children to a given position
    void setNodeAndChildrenPos(decltype(tree)::iterator node, float newX, float newY);

    //The selected number to be added/removed
    float selectedNumber = 0.0;

    //Whether the tree should be self-balancing
    bool selfBalancing = true;

    //Adds a number to the tree
    bool add_number(float number);

    //Removes a number from the tree
    bool remove_number(float number);

    //Adds a random number to the tree
    bool add_random_number();

    //Clears the entire tree
    void clear();

protected:

    //Called when the tree is being saved to a file
    void onSave(std::ostream &outputStream) const override;

    //Called when the tree is being loaded from a file
    void onLoad(std::istream &inputStream) override;

    //Called when a node is being loaded from a file
    void onLoad(decltype(tree)::iterator node, std::istream &inputStream);

    //Called when the tree is being saved to a file
    void onSave(decltype(tree)::const_iterator root, std::ostream &outputStream) const;

public:

    //Called once every frame
    void update(double dt) override;

    //Called to render the binary search tree to the screen
    void render() override;

    //Gets the name of the algorithm
    const std::string &getName() const override;

    //Constructor
    BinarySearchTreeRenderer();
};