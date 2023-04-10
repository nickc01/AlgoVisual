#pragma once
#include "visual_container.h"
#include <OptionRenderers/OptionRenderer.h>
#include <graph.h>

class GraphRenderer : public OptionRenderer {
    static constexpr float NODE_ANGLE_SIZE = 500;
    graph<visual_container<float>> numberGraph{};

    bool mouseDown = false;
    bool movingNode = false;
    //Vector2 mousePosition{};
    int mouseX;
    int mouseY;
    visual_container<float>* selectedNode = nullptr;

    float selectedNumber = 0;
    float connectionNumber = 0;

    void createStarterList();

    void push(float value);
    void pop(float value);
    void clear();
    void connect(float value);
    void disconnect(float value);
protected:
    void onSave(std::ostream& outputStream) const override;
    void onLoad(std::istream& inputStream) override;
public:
    void update(double dt) override;

    void render() override;

    const std::string& getName() const override;

    GraphRenderer();
};