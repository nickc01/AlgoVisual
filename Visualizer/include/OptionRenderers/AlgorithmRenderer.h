#pragma once
#include "options.h"
#include <OptionRenderers/OptionRenderer.h>
#include <functional>
#include <linked_list.h>


class AlgorithmRenderer : public OptionRenderer {
private:
    linked_list<float> numberList;

public:
    using SortType = std::function<void(linked_list<float>&)>;

    SortType sortFunc;

    AlgorithmRenderer(Option option, SortType&& sort);

    void update(double dt) override;

    void render() override;
};