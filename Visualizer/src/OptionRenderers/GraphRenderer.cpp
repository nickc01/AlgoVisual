#include "OptionRenderers/OptionRenderer.h"
#include "raymath.h"
#include <OptionRenderers/GraphRenderer.h>
#include <array>

namespace {
    std::string _name = "Graph";
}

GraphRenderer::GraphRenderer() : OptionRenderer() {
    createStarterList();
}


void GraphRenderer::update(double dt) {

    for (auto& value : numberGraph) {
        value.value.x = Lerp(value.value.x, value.value.targetX, INTERPOLATION_SPEED * dt);
        value.value.y = Lerp(value.value.y, value.value.targetY, INTERPOLATION_SPEED * dt);
    }

    if (!mouseDown) {
        if (IsMouseButtonPressed(0)) {
            mouseDown = true;
            mouseX = GetMouseX();
            mouseY = GetMouseY();

            Vector2 mouseWorldPos{};
            mouseWorldPos.x = GetMouseX();
            mouseWorldPos.y = GetMouseY();
            for (auto& node : numberGraph) {
                auto nodePos = transformPosition(node.value.x,node.value.y);
                if (Vector2Distance(mouseWorldPos, nodePos) <= (CIRCLE_SIZE / scale)) {
                    selectedNode = &node.value;
                    movingNode = true;
                    break;
                }
            }
        }
    }
    else {

        auto newMouseX = GetMouseX();
        auto newMouseY = GetMouseY();
        //auto newMousePosition = transformPosition(GetMouseX(), GetMouseY());
         
        Vector2 mouseDelta{};
        mouseDelta.x = (newMouseX - mouseX) * scale;
        mouseDelta.y = (newMouseY - mouseY) * scale;
        //auto mouseDelta = transformMouseDelta(newMouseX - mouseX, newMouseY - mouseY);

        if (movingNode && selectedNode != nullptr) {
            //selectedNode->targetX = 
            selectedNode->targetX += mouseDelta.x;
            selectedNode->targetY += mouseDelta.y;
        }

        //mousePosition = newMousePosition;
        mouseX = newMouseX;
        mouseY = newMouseY;

        if (IsMouseButtonReleased(0)) {
            mouseDown = false;
            movingNode = false;
        }
    }

    if (!movingNode) {
        OptionRenderer::update(dt);
    }
}

void GraphRenderer::render() {
    //Draw lines firsts
    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {
        auto& node = i->value;
        auto circlePos = transformPosition(node.x, node.y);

        for (auto& connection : *i) {
            auto connectionPos = transformPosition(connection.value.x, connection.value.y);
            DrawLineEx(circlePos, connectionPos, 20 / scale, ORANGE);
        }
    }

    //Draw node circles
    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {
        auto& node = i->value;
        auto circlePos = transformPosition(node.x, node.y);

        if (selectedNode == &node) {
            DrawCircle(circlePos.x, circlePos.y, (CIRCLE_SIZE + 10) / scale, BLACK);
        }

        DrawCircle(circlePos.x, circlePos.y, CIRCLE_SIZE / scale, RED);

        auto str = std::to_string(node.value);

        float textWidth = MeasureText(str.c_str(), TEXT_SIZE);

        auto textPos = transformPosition(node.x - (textWidth / 2), node.y - (TEXT_SIZE / 2));

        DrawTextEx(GetFontDefault(), str.c_str(), textPos, TEXT_SIZE / scale, (TEXT_SIZE / scale) / 10.0, WHITE);
    }
}

const std::string& GraphRenderer::getName() const {
    return _name;
}

void GraphRenderer::createStarterList() {
    constexpr std::array<float,11> numbersToAdd {5,1,2,11,15,0,4,3,9,2,13};

    int counter = 0;
    for (double angle = 0; angle <= 2 * PI; angle += (2.0 * PI) / numbersToAdd.size()) {
        if (counter >= numbersToAdd.size()) {
            break;
        }
        numberGraph.add_node(visual_container<float>(numbersToAdd[counter],cos(angle) * NODE_ANGLE_SIZE,sin(angle) * NODE_ANGLE_SIZE));

        counter++;
    }

    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {
        for (auto j = numberGraph.begin(); j != numberGraph.end(); j++) {
            if (i == j) {
                continue;
            }
            if (rand() % 100 > 80) {
                //i->add_connection(j);
                i->add_connection_to(j);
            }
        }
    }

    //selectedNumber = numberGraph.end();

    /*numberList.add_node(visual_container<float>(5,-5 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(1,-4 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(2,-3 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(11,-2 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(15,-1 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(0,0 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(4,1 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(3,2 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(9,3 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(2,4 * CIRCLE_SPACING,0));
    numberList.add_node(visual_container<float>(13,5 * CIRCLE_SPACING,0));*/
}