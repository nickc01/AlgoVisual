#include "options.h"
#pragma once
#include <OptionRenderers/OptionRenderer.h>
#include <functional>


class AlgorithmRenderer : public OptionRenderer {
private:
    std::vector<float> numberList;

public:
    using SortType = std::function<void(std::vector<float>&)>;

    SortType sortFunc;

    AlgorithmRenderer(SortType&& sort);

    void update(double dt) override;

    void render() override;
};