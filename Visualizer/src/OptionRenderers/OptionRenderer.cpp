#include "raylib.h"
#include "raymath.h"
#include <ios>
#include <iostream>
#include <OptionRenderers/OptionRenderer.h>
#include <global.h>
#include <fstream>
#include <memory>


OptionRenderer::OptionRenderer() {}


void OptionRenderer::update(double dt) {
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

    if (IsMouseButtonPressed(0)) {
        mouseHeld = true;
        lastMousePosX = GetMouseX();
        lastMousePosY = GetMouseY();
    }

    if (mouseHeld) {
        int currentMouseX = GetMouseX();
        int currentMouseY = GetMouseY();

        auto movementAmount = transformMouseDelta(currentMouseX - lastMousePosX, currentMouseY - lastMousePosY);

        dest_cam_x += movementAmount.x;
        dest_cam_y += movementAmount.y;
        lastMousePosX = currentMouseX;
        lastMousePosY = currentMouseY;
    }


    if (IsMouseButtonReleased(0)) {
        mouseHeld = false;
    }

    dest_cam_x += horizontalSpeed;
    dest_cam_y += verticalSpeed;


    cam_x = Lerp(cam_x,dest_cam_x,CAM_INTERP_SPEED * dt);
    cam_y = Lerp(cam_y,dest_cam_y,CAM_INTERP_SPEED * dt);
}

Vector2 OptionRenderer::transformPosition(float x, float y) const {
    Vector2 vector;
    vector.x = (GetScreenWidth() / 2.0) + ((x + cam_x) / scale) + (cam_x / scale);
    vector.y = (GetScreenHeight() / 2.0) + ((y + cam_y) / scale) + (cam_y / scale);
    return vector;
}

Vector2 OptionRenderer::transformMouseDelta(float diffX, float diffY) const {
    Vector2 result;
    result.x = diffX * MOUSE_MOVE_MULTIPLIER * scale;
    result.y = diffY * MOUSE_MOVE_MULTIPLIER * scale;
    return result;
}

void OptionRenderer::save(const char* file_path) const {
    if (file_path == nullptr) {
        return;
    }
    std::ofstream saveFile{file_path};
    try {
        onSave(saveFile);

        saveFile.flush();
        saveFile.close();
    } catch (...) {
        saveFile.close();
        throw;
    }
}

void OptionRenderer::load(const char* file_path) {
    if (file_path == nullptr) {
        return;
    }
    std::ifstream loadFile{file_path};
    try {
        onLoad(loadFile);

        loadFile.close();
    } catch (...) {
        loadFile.close();
        throw;
    }
}

void OptionRenderer::save() const {
    save(saveFile("data.dat"));
}

void OptionRenderer::load() {
    load(loadFile());
}

std::shared_ptr<char> buffer = std::shared_ptr<char>(new char[2000],std::default_delete<char[]>());

std::shared_ptr<char> OptionRenderer::getBuffer() {
    return buffer;
}

void OptionRenderer::DrawTexture(Texture2D tex, Vector2 position, float scale) {
    Vector2 texturePos;
    auto texScaleX = scale * (450 / CIRCLE_SIZE);
    auto texScaleY = scale * (450 / CIRCLE_SIZE);
    texturePos.x = position.x - (tex.width / texScaleX / 2);
    texturePos.y = position.y - (tex.height / texScaleY / 2);
    DrawTextureEx(tex, texturePos, 0, 1.0 / ((texScaleX + texScaleY) / 2.0), WHITE);
}

void OptionRenderer::DrawBackground(float relativeScale, float parallax) {
    auto tex = getGridTexture();

    auto scale = this->scale * relativeScale;

	Rectangle sourceRect;
	sourceRect.x = ((-cam_x * 2 / scale) - GetScreenWidth() / 2.0) * scale;
	sourceRect.y = ((-cam_y * 2 / scale) - GetScreenHeight() / 2.0) * scale;
	sourceRect.width = GetScreenWidth() * scale;
	sourceRect.height = GetScreenHeight() * scale;

    Rectangle destRect;
	destRect.x = GetScreenWidth() / 2.0;
	destRect.y = GetScreenHeight() / 2.0;
	destRect.width = GetScreenWidth();
	destRect.height = GetScreenHeight();

	Vector2 origin;
	origin.x = GetScreenWidth() / 2.0;
	origin.y = GetScreenHeight() / 2.0;
	DrawTexturePro(tex,sourceRect,destRect,origin,0,WHITE);
}