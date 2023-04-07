#pragma once
#include "visual_container.h"
#include <OptionRenderers/OptionRenderer.h>
#include <graph.h>

class GraphRenderer : public OptionRenderer {
    static constexpr float CIRCLE_SIZE = 200;
    graph2<visual_container<float>> numberGraph;

    void createStarterList();
public:

    void update(double dt) override;

    void render() override;

    const std::string& getName() const override;

    GraphRenderer();
};