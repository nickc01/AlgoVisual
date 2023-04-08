#pragma once
#include "quick_sort.h"
#include "visual_container.h"
#include <options.h>
#include <OptionRenderers/OptionRenderer.h>
#include <functional>
#include <linked_list.h>

#define ALGORITHM(FUNC) FUNC<AlgorithmRenderer::iterType,AlgorithmRenderer::comparerType>

class AlgorithmRenderer : public OptionRenderer {
public:
    /*struct valueContainer {
        float value;
        float x;
        float y;

        float targetX;
        float targetY;

        valueContainer(float Value, float X, float Y) : value(Value), x(X), y(Y), targetX(X), targetY(Y) {}
    };*/

    static constexpr bool visualValueComparer(const visual_container<float>& a, const visual_container<float>& b) {
        return a.value < b.value;
    }

private:
    linked_list<visual_container<float>> numberList;
    std::string _name;
    std::string _description;
    float selectedNumber = 0.0;

    void push_front(float value);
    void push_back(float value);

    void pop_front();
    void pop_back();

    void pop(float value);

    void clear();

    void shuffle();
    
    void createStarterList();

    void swap(decltype(numberList.begin())& a, decltype(numberList.begin())& b);
public:
    using iterType = decltype(numberList.begin());
    using valueType = decltype(*numberList.begin());
    using comparerType = std::function<decltype(visualValueComparer)>;
    using AlgoType = std::function<decltype(ALGORITHM(quick_sort))>;
    AlgoType algorithmFunc;

    template<typename A = std::string&, typename B = std::string&>
    AlgorithmRenderer(A name, B description, AlgoType&& algorithm) : OptionRenderer(), _name(std::forward<A>(name)), _description(std::forward<B>(description)), algorithmFunc(std::move(algorithm)) {
        createStarterList();
    }

    void update(double dt) override;

    void render() override;

    const std::string& getName() const override;
};