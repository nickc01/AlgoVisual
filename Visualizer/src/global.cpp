#include "nfd.h"
#include "raylib.h"
#include <algorithm>
#include <exception>
#include <global.h>
#include <vector>
#include <thread>
#include <mutex>
#include <nfd.h>
#include <filesystem>
#include <iostream>


long lockCounter = 0;
std::vector<long> uiLocks{};

long sortLock = 0;
std::thread sortThread{};
bool runningSortThread = false;
bool stoppingThread = false;
nfdchar_t *outPath;
Texture2D orange_circle_tex;
Texture2D grid_texture;

std::recursive_mutex sortMutex{};

nfdfilteritem_t filterItem[1] = { { "Data File", "dat" }};

/*void test() {
    SetWindowSize(200, 400);
}*/

int windowSizeX() {
    return GetScreenWidth();
}

int windowSizeY() {
    return GetScreenHeight();
}

bool beginSort(std::function<void()>&& sortFunc) {
    {
        std::lock_guard<decltype(sortMutex)> guard{sortMutex};
        if (sortLock != 0) {
            return false;
        }

        sortLock = addUILock();
        runningSortThread = true;
        stoppingThread = false;

        if (sortThread.joinable()) {
            sortThread.join();
        }
    }

    sortThread = std::thread{[func = std::move(sortFunc)]() {
        try {
            func();
        } catch (...) {
            //Exceptions are used to stop a sort
        }
        if (removeUILock(sortLock)) {
            sortLock = 0;
        }
        runningSortThread = false;
        stoppingThread = false;
    }};
    return true;
}

bool endSort() {
    std::lock_guard<decltype(sortMutex)> guard{sortMutex};
    stoppingThread = true;
    return true;
}

long addUILock() {
    std::lock_guard<decltype(sortMutex)> guard{sortMutex};
	auto number = ++lockCounter;
	uiLocks.push_back(number);
	return number;
}

bool removeUILock(long number) {
    std::lock_guard<decltype(sortMutex)> guard{sortMutex};
	auto result = std::find(uiLocks.begin(),uiLocks.end(),number);
	if (result != uiLocks.end()) {
			uiLocks.erase(result);
			return true;
	}
	return false;
}

bool uiLocked() {
    std::lock_guard<decltype(sortMutex)> guard{sortMutex};
	return uiLocks.size() != 0;
}

bool runningSort() {
    return runningSortThread;
}

bool stoppingSort() {
    return stoppingThread;
}

std::thread& getSortThread() {
    return sortThread;
}


const char* saveFile(const char* defaultName) {
    nfdresult_t result = NFD_SaveDialog(&outPath,filterItem,1,NULL,defaultName);
    if (result == NFD_OKAY) {
        return outPath;
    }
    else {
        return nullptr;
    }
}

const char* loadFile() {
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result == NFD_OKAY) {
        return outPath;
    }
    else {
        return nullptr;
    }
}

void loadTextures() {
    orange_circle_tex = LoadTexture(RES_ORANGE_CIRCLE);
    SetTextureFilter(orange_circle_tex, TEXTURE_FILTER_TRILINEAR);

    grid_texture = LoadTexture(RES_GRID_TEXTURE);
    SetTextureFilter(grid_texture, TEXTURE_FILTER_POINT);
    SetTextureWrap(grid_texture,TEXTURE_WRAP_REPEAT);
}

Texture2D& getOrangeCircle() {
    return orange_circle_tex;
}

Texture2D& getGridTexture() {
    return grid_texture;
}

void unloadTextures() {
    UnloadTexture(orange_circle_tex);
}