// Include necessary header files
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <fstream>
#include <memory>

// Include custom header files
#include <OptionRenderers/OptionRenderer.h>
#include <global.h>

// Initialize OptionRenderer constructor
OptionRenderer::OptionRenderer() {}

// Update function to handle input and update camera position and scale
void OptionRenderer::update(double dt) {
    // Handle scaling with mouse wheel
    auto movement = -GetMouseWheelMove();
    dest_scale += movement * SCALE_MOVE_SPEED;
    dest_scale = Clamp(dest_scale, MIN_SCALE, MAX_SCALE);
    scale = Lerp(scale, dest_scale, SCALE_LERP_SPEED * dt);
    scale = Clamp(scale, MIN_SCALE, MAX_SCALE);


    // Handle camera movement with arrow keys and WASD
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

    // Handle mouse input for camera movement
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

    // Interpolate camera position for smooth movement
    cam_x = Lerp(cam_x, dest_cam_x, CAM_INTERP_SPEED * dt);
    cam_y = Lerp(cam_y, dest_cam_y, CAM_INTERP_SPEED * dt);
}

// Transform a position from game coordinates to screen coordinates
Vector2 OptionRenderer::transformPosition(float x, float y) const {
    Vector2 vector;
    vector.x = (GetScreenWidth() / 2.0) + ((x + cam_x) / scale) + (cam_x / scale);
    vector.y = (GetScreenHeight() / 2.0) + ((y + cam_y) / scale) + (cam_y / scale);
    return vector;
    }

// Transform the change in mouse position to a movement vector for the camera
Vector2 OptionRenderer::transformMouseDelta(float diffX, float diffY) const {
    Vector2 result;
    result.x = diffX * MOUSE_MOVE_MULTIPLIER * scale;
    result.y = diffY * MOUSE_MOVE_MULTIPLIER * scale;
    return result;
}

// This function takes a file path and saves the current state of the OptionRenderer object to the file
// If the file path is null, the function returns without saving anything
void OptionRenderer::save(const char* file_path) const {
    if (file_path == nullptr) {
        return;
    }
    // Creates a new ofstream object using the file path
    std::ofstream saveFile{file_path};
    try {
        // Calls the onSave function with the ofstream object as a parameter to write the object state to the file
        onSave(saveFile);

        // Flushes thes output buffer to ensure all data is written to the file
        saveFile.flush();
        // Closes the file
        saveFile.close();
    } catch (...) {
        // If an exception is thrown during the save process, the file is closed and the exception is rethrown
        saveFile.close();
        throw;
    }
}

// This function loads the state of the OptionRenderer object from a file specified by the file path parameter
// If the file path is null, the function returns without loading anything
void OptionRenderer::load(const char* file_path) {
    if (file_path == nullptr) {
        return;
    }
    // Creates a new ifstream object using the file path
    std::ifstream loadFile{file_path};
    try {
        // Calls the onLoad function with the ifstream object as a parameter to load the object state from the file
        onLoad(loadFile);

        // Closes the file
        loadFile.close();
    } catch (...) {
        // If an exception is thrown during the load process, the file is closed and the exception is rethrown
        loadFile.close();
        throw;
    }
    }

// This function calls the save function with a default file path of "data.dat"
void OptionRenderer::save() const {
    // Calls the save function with the default file path
    save(saveFile("data.dat"));
}

// This function calls the load function with a default file path returned by the loadFile function
void OptionRenderer::load() {
    // Calls the load function with the default file path
    load(loadFile());
}

// Creates a new shared pointer object for a character buffer of size 2000
std::shared_ptr<char> buffer = std::shared_ptr<char>(new char[2000],std::default_delete<char[]>());

// This function returns the shared pointer object for the character buffer
std::shared_ptr<char> OptionRenderer::getBuffer() {
    return buffer;
}

// This function takes a texture, a position, and a scale, and draws the texture at the position with the specified scale
void OptionRenderer::DrawTexture(Texture2D tex, Vector2 position, float scale) {
    // Calculates the texture position based on the position, texture scale, and circle size constants
    Vector2 texturePos;
    auto texScaleX = scale * (450 / CIRCLE_SIZE);
    auto texScaleY = scale * (450 / CIRCLE_SIZE);
    texturePos.x = position.x - (tex.width / texScaleX / 2);
    texturePos.y = position.y - (tex.height / texScaleY / 2);
    // Draws the texture using the DrawTextureEx function with a calculated scale and the color white
    DrawTextureEx(tex, texturePos, 0, 1.0 / ((texScaleX + texScaleY) / 2.0), WHITE);
}

// This function draws the background of the option screen.
void OptionRenderer::DrawBackground(float relativeScale) {
    // Get the grid texture.
    auto tex = getGridTexture();

    // Calculate the new scale based on the relative scale passed in.
    auto scale = this->scale * relativeScale;

    // Calculate the source rectangle based on the camera position, scale, and screen size.
    Rectangle sourceRect;
    sourceRect.x = ((-cam_x * 2 / scale) - GetScreenWidth() / 2.0) * scale;
    sourceRect.y = ((-cam_y * 2 / scale) - GetScreenHeight() / 2.0) * scale;
    sourceRect.width = GetScreenWidth() * scale;
    sourceRect.height = GetScreenHeight() * scale;

    // Calculate the destination rectangle based on the screen size.
    Rectangle destRect;
    destRect.x = GetScreenWidth() / 2.0;
    destRect.y = GetScreenHeight() / 2.0;
    destRect.width = GetScreenWidth();
    destRect.height = GetScreenHeight();

    // Set the origin of the texture to the center of the screen.
    Vector2 origin;
    origin.x = GetScreenWidth() / 2.0;
    origin.y = GetScreenHeight() / 2.0;

    // Draw the texture using the calculated source and destination rectangles, origin, and color.
    DrawTexturePro(tex, sourceRect, destRect, origin, 0, WHITE);
}