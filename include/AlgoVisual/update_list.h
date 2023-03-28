#pragma once
#include <AlgoVisual/Event.h>

class update_list {
	Event<void,double> updatables;
public:
	template <typename T>
	unsigned long add(T& updater) {
		return updatables.add(std::move(std::bind(&T::update, &updater, std::placeholders::_1)));
	}

	bool remove(unsigned long id) {
		return updatables.remove(id);
	}

	void update(double dt) {
		updatables.call(dt);
	}
};