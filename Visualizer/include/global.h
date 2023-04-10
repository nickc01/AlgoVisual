#pragma once
#include <memory>
#include <raylib.h>
#include <functional>
#include <thread>

int windowSizeX();
int windowSizeY();



bool beginSort(std::function<void()>&& sortFunc);

bool endSort();

long addUILock();
bool removeUILock(long number);
bool uiLocked();

bool runningSort();
bool stoppingSort();

std::thread& getSortThread();

const char* saveFile(const char* defaultName);

const char* loadFile();


void loadTextures();

Texture2D& getOrangeCircle();
Texture2D& getGridTexture();

void unloadTextures();

