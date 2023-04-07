#include "OptionRenderers/OptionRenderer.h"
#include "common.h"
#include "global.h"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include <OptionRenderers/AlgorithmRenderer.h>
#include <chrono>
#include <functional>
#include <string>
#include <thread>

/*AlgorithmRenderer::AlgorithmRenderer(std::string name, std::string description, AlgoType&& algorithm) : OptionRenderer(), _name(name), _description(description), algorithmFunc(std::move(algorithm)) {
    createStarterList();
}*/


void AlgorithmRenderer::update(double dt) {
    OptionRenderer::update(dt);
    for (auto& value : numberList) {
        value.x = Lerp(value.x, value.targetX, INTERPOLATION_SPEED * dt);
        value.y = Lerp(value.y, value.targetY, INTERPOLATION_SPEED * dt);
    }
}

void AlgorithmRenderer::render() {
    ImGui::TextWrapped("%s", _description.c_str());
    if (!uiLocked()) {
        ImGui::InputFloat("Number",&selectedNumber);
        if (ImGui::Button("Add Number To Start")) {
            push_front(selectedNumber);
        }

        if (ImGui::Button("Add Number To End")) {
            push_back(selectedNumber);
        }

        if (ImGui::Button("Remove Number")) {
            pop(selectedNumber);
        }

        if (ImGui::Button("Remove Number from end")) {
            pop_back();
        }

        if (ImGui::Button("Remove Number from front")) {
            pop_front();
        }

        if (ImGui::Button("Add Random Number")) {
            push_back((rand() % 2000) - 1000);
        }

        if (ImGui::Button("Shuffle")) {
            shuffle();
        }

        if (ImGui::Button("Clear")) {
            clear();
        }

        if (ImGui::Button("Sort")) {
            beginSort([&](){
                #ifdef __cpp_lib_bind_front
                    algorithmFunc(numberList.begin(),numberList.end(),comparerType(visualValueComparer),std::bind_front(&AlgorithmRenderer::swap, *this));
                #else
                    algorithmFunc(numberList.begin(),numberList.end(),comparerType(visualValueComparer),std::bind(&AlgorithmRenderer::swap,*this, std::placeholders::_1,std::placeholders::_2));
                #endif
            });
        }
    }
    else {
        if (ImGui::Button("Stop Sorting")) {
            endSort();
        }
    }

    for (auto value : numberList) {

        auto circlePos = transformPosition(value.x, value.y);


        DrawCircle(circlePos.x,circlePos.y,CIRCLE_SIZE / scale,RED);

        auto str = std::to_string(value.value);

        float textWidth = MeasureText(str.c_str(),TEXT_SIZE);

        auto textPos = transformPosition(value.x  - (textWidth / 2), value.y - (TEXT_SIZE / 2));

        DrawTextEx(GetFontDefault(), str.c_str(),textPos, TEXT_SIZE / scale, (TEXT_SIZE / scale) / 10.0, WHITE);
    }
}

const std::string& AlgorithmRenderer::getName() const {
    return _name;
}

void AlgorithmRenderer::swap(decltype(numberList.begin())& a, decltype(numberList.begin())& b) {
    if (stoppingSort())
    {
        throw std::exception();
    }
    visual_container<float> temp = *a;
    a->value = b->value;
    a->x = b->x;
    a->y = b->y;

    b->value = temp.value;
    b->x = temp.x;
    b->y = temp.y;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void AlgorithmRenderer::createStarterList() {
    numberList.push_back(visual_container<float>(5,-5 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(1,-4 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(2,-3 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(11,-2 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(15,-1 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(0,0 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(4,1 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(3,2 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(9,3 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(2,4 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(13,5 * CIRCLE_SPACING,0));
}

void AlgorithmRenderer::push_front(float value) {
    auto oldFront = numberList.front();

    float targetX = oldFront != numberList.end() ? oldFront->targetX : CIRCLE_SPACING;
    float targetY = oldFront != numberList.end() ? oldFront->targetY : 0;

    visual_container<float> newValue = visual_container<float> {value,targetX,targetY};
    newValue.targetX -= CIRCLE_SPACING;
    numberList.push_front(std::move(newValue));
}
void AlgorithmRenderer::push_back(float value) {
    auto oldBack = numberList.back();

    float targetX = oldBack != numberList.end() ? oldBack->targetX : -CIRCLE_SPACING;
    float targetY = oldBack != numberList.end() ? oldBack->targetY : 0;

    visual_container<float> newValue = visual_container<float> {value,targetX,targetY};
    newValue.targetX += CIRCLE_SPACING;
    numberList.push_back(std::move(newValue));
}

void AlgorithmRenderer::pop_front() {
    numberList.pop_front();
}

void AlgorithmRenderer::pop_back() {
    numberList.pop_back();
}

void AlgorithmRenderer::pop(float value) {
    for (auto i = numberList.begin(); i != numberList.end(); i++)
    {
        if (i->value == value)
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

void AlgorithmRenderer::clear() {
    dest_cam_x = 0;
    dest_cam_y = 0;
    cam_x = 0;
    cam_y = 0;
    numberList.clear();
}

void AlgorithmRenderer::shuffle() {
    for (int times = 0; times < numberList.getSize(); times++) {
        auto begin = numberList.begin();
        for (int i = 0; i < numberList.getSize(); i++) {
            auto destination = numberList.begin();
            for (int j = (rand() % numberList.getSize()); j > 0; j--) {
                ++destination;
            }
            visual_container<float> temp = *begin;
            begin->value = destination->value;
            begin->x = destination->x;
            begin->y = destination->y;

            destination->value = temp.value;
            destination->x = temp.x;
            destination->y = temp.y;
        }
    }
}