#pragma once
#include <string>

class OptionRenderer {
public:
    OptionRenderer();

    virtual void update(double dt) = 0;

    virtual void render() = 0;

    virtual const std::string& getName() const = 0;
};