#pragma once
#include "raylib.h"
#include <string>

class OptionRenderer {
protected:
    float scale;
    float cam_x;
    float cam_y;
public:
    float dest_scale;
    float dest_cam_x;
    float dest_cam_y;
    static constexpr float MIN_SCALE = 0.25;
    static constexpr float MAX_SCALE = 5.0;
    static constexpr float SCALE_LERP_SPEED = 7;
    static constexpr float SCALE_MOVE_SPEED = 0.07;
    static constexpr float CAM_INTERP_SPEED = 7;
    static constexpr float CAM_MOVE_SPEED = 300;


    OptionRenderer();

    virtual void update(double dt);

    virtual void render() = 0;

    virtual const std::string& getName() const = 0;

    Vector2 transformPosition(float x, float y) const;
};