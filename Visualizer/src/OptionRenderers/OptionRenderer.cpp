#include <OptionRenderers/OptionRenderer.h>

OptionRenderer::OptionRenderer(Option option) : option(option) {

}

Option OptionRenderer::getOption() const {
    return option;
}