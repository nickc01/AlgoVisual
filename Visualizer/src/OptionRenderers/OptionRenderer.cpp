#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <OptionRenderers/OptionRenderer.h>
#include <global.h>

OptionRenderer::OptionRenderer() {}


void OptionRenderer::update(double dt) {
    //std::cout << "Scale = " << scale << std::endl;
    auto movement = -GetMouseWheelMove();
    dest_scale += movement * SCALE_MOVE_SPEED;

    dest_scale = Clamp(dest_scale, MIN_SCALE, MAX_SCALE);

    scale = Lerp(scale,dest_scale,SCALE_LERP_SPEED * dt);

    scale = Clamp(scale,MIN_SCALE,MAX_SCALE);


    float horizontalSpeed = 0;
    float verticalSpeed = 0;

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        verticalSpeed -= CAM_MOVE_SPEED * dt;
    }

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        verticalSpeed += CAM_MOVE_SPEED * dt;
    }

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        horizontalSpeed -= CAM_MOVE_SPEED * dt;
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        horizontalSpeed += CAM_MOVE_SPEED * dt;
    }

    dest_cam_x += horizontalSpeed;
    dest_cam_y += verticalSpeed;


    cam_x = Lerp(cam_x,dest_cam_x,CAM_INTERP_SPEED * dt);
    cam_y = Lerp(cam_y,dest_cam_y,CAM_INTERP_SPEED * dt);



    //scale = dest_scale;
    //scale = 0.5;
}

Vector2 OptionRenderer::transformPosition(float x, float y) const {
    Vector2 vector;
    vector.x = (GetScreenWidth() / 2) + ((x + cam_x) / scale) + (cam_x / scale);
    vector.y = (GetScreenHeight() / 2) + ((y + cam_y) / scale) + (cam_y / scale);

    return vector;
}