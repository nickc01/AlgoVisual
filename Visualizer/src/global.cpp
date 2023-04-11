// Including necessary headers
#include "nfd.h"
#include "raylib.h"
#include <algorithm>
#include <exception>
#include <filesystem>
#include <global.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Global variables
long lockCounter = 0;        // Counter for UI locks
std::vector<long> uiLocks{}; // Vector to hold UI locks

long sortLock = 0;              // Lock for sorting thread
std::thread sortThread{};       // Sorting thread
bool runningSortThread = false; // Flag to check if sorting thread is running
bool stoppingThread = false;    // Flag to check if sorting thread is stopping
nfdchar_t *outPath;             // Output path for file dialogs
Texture2D orange_circle_tex;    // Orange circle texture
Texture2D grid_texture;         // Grid texture

std::recursive_mutex sortMutex{}; // Mutex for sorting thread

nfdfilteritem_t filterItem[1] = {
    {"Data File", "dat"}}; // Filter item for file dialogs

// Function to start sorting thread
bool beginSort(std::function<void()> &&sortFunc) {
  {
    std::lock_guard<decltype(sortMutex)> guard{sortMutex};
    if (sortLock != 0) { // Check if sortLock is already locked
      return false;
    }

    sortLock = addUILock(); // Add a UI lock
    runningSortThread = true; // Set flag to indicate that sorting thread is running
    stoppingThread = false; // Set flag to indicate that sorting thread is not stopping

    if (sortThread.joinable()) { // Join the previous sorting thread, if any
      sortThread.join();
    }
  }

  // Start a new sorting thread
  sortThread = std::thread{[func = std::move(sortFunc)]() {
    try {
      func(); // Call the sorting function
    } catch (...) {
      // Exceptions are used to stop a sort
    }
    if (removeUILock(sortLock)) { // Remove the UI lock
      sortLock = 0;
    }
    runningSortThread =
        false; // Set flag to indicate that sorting thread is not running
    stoppingThread =
        false; // Set flag to indicate that sorting thread is not stopping
  }};
  return true;
}

// Function to stop sorting thread
bool endSort() {
  std::lock_guard<decltype(sortMutex)> guard{sortMutex};
  stoppingThread = true; // Set flag to indicate that sorting thread is stopping
  return true;
}

// Function to add a UI lock
long addUILock() {
  std::lock_guard<decltype(sortMutex)> guard{sortMutex};
  auto number = ++lockCounter;
  uiLocks.push_back(number);
  return number;
}

// Function to remove a UI lock
bool removeUILock(long number) {
  std::lock_guard<decltype(sortMutex)> guard{sortMutex};
  auto result = std::find(uiLocks.begin(), uiLocks.end(), number);
  if (result != uiLocks.end()) {
    uiLocks.erase(result);
    return true;
  }
  return false;
}

// Function to check if UI is locked
bool uiLocked() {
  std::lock_guard<decltype(sortMutex)> guard{sortMutex};
  return uiLocks.size() != 0;
}

// Function to check if sorting thread is running
bool runningSort() { return runningSortThread; }

// Function to check if sorting thread is stopping
bool stoppingSort() { return stoppingThread; }

// Returns a reference to the sort thread
std::thread &getSortThread() { return sortThread; }

// Prompts the user to save a file and returns the path
// Returns nullptr if the user cancels the operation
const char *saveFile(const char *defaultName) {
  nfdresult_t result =
      NFD_SaveDialog(&outPath, filterItem, 1, NULL, defaultName);
  if (result == NFD_OKAY) {
    return outPath;
  } else {
    return nullptr;
  }
}

// Prompts the user to load a file and returns the path
// Returns nullptr if the user cancels the operation
const char *loadFile() {
  nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
  if (result == NFD_OKAY) {
    return outPath;
  } else {
    return nullptr;
  }
}

// Loads textures used in the program
void loadTextures() {
  orange_circle_tex = LoadTexture(RES_ORANGE_CIRCLE);
  SetTextureFilter(orange_circle_tex, TEXTURE_FILTER_TRILINEAR);

  grid_texture = LoadTexture(RES_GRID_TEXTURE);
  SetTextureFilter(grid_texture, TEXTURE_FILTER_POINT);
  SetTextureWrap(grid_texture, TEXTURE_WRAP_REPEAT);
}

// Returns a reference to the orange circle texture
Texture2D &getOrangeCircle() { return orange_circle_tex; }

// Returns a reference to the grid texture
Texture2D &getGridTexture() { return grid_texture; }

// Unloads textures used in the program
void unloadTextures() { UnloadTexture(orange_circle_tex); }
