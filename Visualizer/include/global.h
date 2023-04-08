#pragma once
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