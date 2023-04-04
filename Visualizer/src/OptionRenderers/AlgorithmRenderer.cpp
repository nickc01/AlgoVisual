#include "OptionRenderers/OptionRenderer.h"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include <OptionRenderers/AlgorithmRenderer.h>
#include <string>

AlgorithmRenderer::AlgorithmRenderer(std::string name, SortType&& sort) : OptionRenderer(), _name(name), sortFunc(std::move(sort)) {
    createStarterList();
}


void AlgorithmRenderer::update(double dt) {
    OptionRenderer::update(dt);
    for (auto& value : numberList) {
        value.x = Lerp(value.x, value.targetX, INTERPOLATION_SPEED * dt);
        value.y = Lerp(value.y, value.targetY, INTERPOLATION_SPEED * dt);
    }
}

void AlgorithmRenderer::render() {
    ImGui::InputFloat("Number",&selectedNumber);
    if (ImGui::Button("Add Number To Start")) {

        visualValue newValue = visualValue {selectedNumber,numberList.front()->targetX,numberList.front()->targetY};
        newValue.targetX = numberList.front()->targetX - CIRCLE_SPACING;
        numberList.push_front(std::move(newValue));
    }

    if (ImGui::Button("Add Number To End")) {
        visualValue newValue = visualValue {selectedNumber,numberList.back()->targetX,numberList.back()->targetY};
        newValue.targetX = numberList.back()->targetX + CIRCLE_SPACING;
        numberList.push_back(std::move(newValue));
    }

    if (ImGui::Button("Remove Number")) {
        for (auto i = numberList.begin(); i != numberList.end(); i++)
		{
			if (i->value == selectedNumber)
			{
                for (auto j = i; j != numberList.end(); j++) {
                    if (j == i) {
                        continue;
                    }
                    j->targetX -= CIRCLE_SPACING;
                }
				numberList.pop_element(i);
                break;
			}
		}
    }

    for (auto value : numberList) {

        //auto centerX = 400;
        //auto centerY = 300;

        //auto test = (centerX + value.x) / 2;

        auto circlePos = transformPosition(value.x, value.y);


        DrawCircle(circlePos.x,circlePos.y,CIRCLE_SIZE / scale,RED);

        auto str = std::to_string(value.value);

        float textWidth = MeasureText(str.c_str(),TEXT_SIZE);

        auto textPos = transformPosition(value.x  - (textWidth / 2), value.y - (TEXT_SIZE / 2));

        DrawText(str.c_str(),textPos.x,textPos.y,TEXT_SIZE / scale,WHITE);

    }
}

const std::string& AlgorithmRenderer::getName() const {
    return _name;
}

void AlgorithmRenderer::createStarterList() {
    numberList.push_back(visualValue(5,-5 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(1,-4 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(2,-3 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(11,-2 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(15,-1 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(0,0 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(4,1 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(3,2 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(9,3 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(2,4 * CIRCLE_SPACING,0));
    numberList.push_back(visualValue(13,5 * CIRCLE_SPACING,0));
}