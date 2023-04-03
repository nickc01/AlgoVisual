#pragma once
#include <options.h>

class OptionRenderer {
protected:
    Option option;
public:
    virtual void update(double dt) = 0;

    virtual void render() = 0;

    Option getOption() const;
};