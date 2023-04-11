//This file contains the header file for the Graph Renderer. Used for rendering graphs
#pragma once

//Required headers
#include "visual_container.h"
#include <OptionRenderers/OptionRenderer.h>
#include <graph.h>

class GraphRenderer : public OptionRenderer {
    // The angle size of the nodes in the graph
    static constexpr float NODE_ANGLE_SIZE = 500;

    // A graph containing visual containers of float
    graph<visual_container<float>> numberGraph{};

    // Flag to indicate if the mouse is down
    bool mouseDown = false;

    // Flag to indicate if a node is being moved
    bool movingNode = false;

    // The X and Y coordinates of the mouse
    int mouseX;
    int mouseY;

    // Pointer to the selected node
    visual_container<float>* selectedNode = nullptr;

    // The selected number to be added/removed
    float selectedNumber = 0;

    // The number that will be used to connect nodes
    float connectionNumber = 0;

    //Adds a new value to the graph
    void push(float value);

    //Removes the specified value from the graph
    void pop(float value);

    //Clears the entire graph
    void clear();

    //Connects two nodes in the graph
    void connect(float value);

    //Disconnects two nodes in the graph
    void disconnect(float value);

    //Creates a starting list
    void createStarterList();

protected:
    //Called when the graph is being saved to a file
    void onSave(std::ostream& outputStream) const override;

    //Called when the graph is being loaded from a file
    void onLoad(std::istream& inputStream) override;

public:
    //Called once every frame
    void update(double dt) override;

    //Called to render the graph to the screen
    void render() override;

    //Gets the name of the graph
    const std::string& getName() const override;

    //Constructor that creates a starter graph
    GraphRenderer(); 
};