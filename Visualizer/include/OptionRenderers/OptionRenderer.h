#pragma once
#include "raylib.h"
#include <memory>
#include <string>
#include <ostream>
#include <istream>

class OptionRenderer {
public:
    static constexpr float MIN_SCALE = 0.25; //The minimum UI scale
    static constexpr float MAX_SCALE = 5.0; //The maximum UI scale
    static constexpr float SCALE_LERP_SPEED = 7; //How fast the scale value interpolates from one value to another
    static constexpr float SCALE_MOVE_SPEED = 0.07; //How fast the scroll-wheel zooms in and out
    static constexpr float CAM_INTERP_SPEED = 7; //How fast the camera interpolates from one position to another
    static constexpr float CAM_MOVE_SPEED = 300; //How fast the camera moves when using the arrow keys or WASD
    static constexpr float MOUSE_MOVE_MULTIPLIER = 0.5; //How much the mouse moves the camera

    static constexpr float CIRCLE_SIZE = 50; //The size of a circle element
    static constexpr float CIRCLE_SPACING = 100; //The spacing between circle elements
    static constexpr float INTERPOLATION_SPEED = 7; // How fast objects interpolate from one location to another
    static constexpr float TEXT_SIZE = 15; //The size of the text within circles
protected:
    float scale = 1;
    float cam_x = 0;
    float cam_y = 0;

    bool mouseHeld = false;
    int lastMousePosX = 0;
    int lastMousePosY = 0;

    virtual void onSave(std::ostream& outputStream) const = 0;
    virtual void onLoad(std::istream& inputStream) = 0;
public:
    float dest_scale = 1;
    float dest_cam_x = 0;
    float dest_cam_y = 0;

    static void DrawTexture(Texture2D tex, Vector2 position, float scale);

    void DrawBackground(float relativeScale = 1.0);

    OptionRenderer();

    virtual void update(double dt);

    virtual void render() = 0;

    virtual const std::string& getName() const = 0;

    Vector2 transformPosition(float x, float y) const;

    Vector2 transformMouseDelta(float diffX, float diffY) const;

    void save(const char* file_path) const;
    void load(const char* file_path);

    void save() const;
    void load();

    static std::shared_ptr<char> getBuffer();
};