#pragma once
#include <options.h>
#include <OptionRenderers/OptionRenderer.h>
#include <functional>
#include <linked_list.h>


class AlgorithmRenderer : public OptionRenderer {
public:
    static constexpr float CIRCLE_SIZE = 50;
    static constexpr float CIRCLE_SPACING = 100;
    static constexpr float INTERPOLATION_SPEED = 7;
    static constexpr float TEXT_SIZE = 20;
private:
    struct visualValue {
        float value;
        float x;
        float y;

        float targetX;
        float targetY;

        visualValue(float Value, float X, float Y) : value(Value), x(X), y(Y), targetX(X), targetY(Y) {}
    };
    //linked_list<float> numberList {1,9,3,6,2,8,5,7,2,};
    linked_list<visualValue> numberList;
    std::string _name;
    float selectedNumber;

    void push_front();
    void push_end();

    void createStarterList();
public:
    using SortType = std::function<void(linked_list<float>&)>;

    SortType sortFunc;

    AlgorithmRenderer(std::string name, SortType&& sort);

    void update(double dt) override;

    void render() override;

    const std::string& getName() const override;
};