#include <OptionRenderers/AlgorithmRenderer.h>

AlgorithmRenderer::AlgorithmRenderer(Option option, SortType&& sort) : OptionRenderer(option), sortFunc(std::move(sort)) {

}


void AlgorithmRenderer::update(double dt) {

}

void AlgorithmRenderer::render() {

}