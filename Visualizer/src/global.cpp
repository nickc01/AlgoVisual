#include "raylib.h"
#include <algorithm>
#include <exception>
#include <global.h>
#include <vector>
#include <thread>
#include <mutex>


long lockCounter = 0;
std::vector<long> uiLocks{};

long sortLock = 0;
std::thread sortThread{};
bool runningSortThread = false;
bool stoppingThread = false;

std::recursive_mutex sortMutex{};

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