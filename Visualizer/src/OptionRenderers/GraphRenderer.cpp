#include "OptionRenderers/OptionRenderer.h"
#include "raymath.h"
#include <OptionRenderers/GraphRenderer.h>
#include <array>


std::string _name = "Graph";

GraphRenderer::GraphRenderer() : OptionRenderer() {
    createStarterList();
}


void GraphRenderer::update(double dt) {

    for (auto& value : numberGraph) {
        //value.Value.x = Lerp(value.Value.x, value.Value.targetX, INTERPOLATION_SPEED * dt);
        //value.Value.y = Lerp(value.Value.y, value.Value.targetY, INTERPOLATION_SPEED * dt);
    }
    OptionRenderer::update(dt);
}

void GraphRenderer::render() {
    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {

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
        numberGraph.add_node(visual_container<float>(numbersToAdd[counter],cos(angle) * CIRCLE_SIZE,sin(angle) * CIRCLE_SIZE));

        counter++;
    }

    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {
        for (auto j = numberGraph.begin(); j != numberGraph.end(); j++) {
            if (i == j) {
                continue;
            }
            if (rand() % 100 > 80) {
                //i->add_connection(j);
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