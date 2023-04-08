#pragma once
#include "visual_container.h"
#include <OptionRenderers/OptionRenderer.h>
#include <graph.h>

class GraphRenderer : public OptionRenderer {
    static constexpr float NODE_ANGLE_SIZE = 500;
    graph2<visual_container<float>> numberGraph{};

    bool mouseDown = false;
    bool movingNode = false;
    //Vector2 mousePosition{};
    int mouseX;
    int mouseY;
    visual_container<float>* selectedNode = nullptr;

    void createStarterList();
public:

    void update(double dt) override;

    void render() override;

    const std::string& getName() const override;

    GraphRenderer();
};