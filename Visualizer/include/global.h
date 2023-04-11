// This is a C++ header file that contains global miscellaneous functions used for the functionality of the program.

#pragma once // Ensures this header file is only included once during compilation

#include <memory> // Includes shared pointer functionality
#include <raylib.h> // Includes the Raylib game development framework
#include <functional> // Includes function object functionality
#include <thread> // Includes threading functionality

// Begins the sorting process
bool beginSort(std::function<void()>&& sortFunc);

// Ends the sorting process
bool endSort();

// Locks and unlocks the user interface during sorting
long addUILock(); // Returns a unique identifier for the lock
bool removeUILock(long number); // Removes the lock with the given identifier
bool uiLocked(); // Checks if the user interface is currently locked

// Checks if a sorting process is running
bool runningSort();

// Stops the sorting process
bool stoppingSort();

// Gets the sorting thread
std::thread& getSortThread();

// Opens a dialog box to save a file with the given default name and returns the selected file path
const char* saveFile(const char* defaultName);

// Opens a dialog box to load a file and returns the selected file path
const char* loadFile(); 

// Loads textures
void loadTextures();

// Returns the orange circle texture
Texture2D& getOrangeCircle();

// Returns the grid texture
Texture2D& getGridTexture();

// Unloads textures
void unloadTextures();
