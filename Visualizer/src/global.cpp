#include "raylib.h"
#include <global.h>


void test() {
    SetWindowSize(200, 400);
}

int windowSizeX() {
    return GetScreenWidth();
}

int windowSizeY() {
    return GetScreenHeight();
}