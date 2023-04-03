#include <algorithm>
#include <common.h>
#include <vector>

long lockCounter = 0;
std::vector<long> uiLocks;

std::string version() {
	return "1.0.0";
}

long addUILock() {
	auto number = ++lockCounter;
	uiLocks.push_back(number);
	return number;
}

bool removeUILock(long number) {
	auto result = std::find(uiLocks.begin(),uiLocks.end(),number);
	if (result != uiLocks.end()) {
			uiLocks.erase(result);
			return true;
	}
	return false;
}
bool uiLocked() {
	return uiLocks.size() != 0;
}