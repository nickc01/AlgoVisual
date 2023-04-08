#pragma once
#include "raylib.h"
#include <string>

class OptionRenderer {
public:
    static constexpr float MIN_SCALE = 0.25;
    static constexpr float MAX_SCALE = 5.0;
    static constexpr float SCALE_LERP_SPEED = 7;
    static constexpr float SCALE_MOVE_SPEED = 0.07;
    static constexpr float CAM_INTERP_SPEED = 7;
    static constexpr float CAM_MOVE_SPEED = 300;
    static constexpr float MOUSE_MOVE_MULTIPLIER = 0.5;

    static constexpr float CIRCLE_SIZE = 50;
    static constexpr float CIRCLE_SPACING = 100;
    static constexpr float INTERPOLATION_SPEED = 7;
    static constexpr float TEXT_SIZE = 15;
protected:
    float scale = 1;
    float cam_x = 0;
    float cam_y = 0;

    bool mouseHeld = false;
    int lastMousePosX = 0;
    int lastMousePosY = 0;
public:
    float dest_scale = 1;
    float dest_cam_x = 0;
    float dest_cam_y = 0;



    OptionRenderer();

    virtual void update(double dt);

    virtual void render() = 0;

    virtual const std::string& getName() const = 0;

    Vector2 transformPosition(float x, float y) const;

    Vector2 transformMouseDelta(float diffX, float diffY) const;
};