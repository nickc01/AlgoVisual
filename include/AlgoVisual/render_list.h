#pragma once
#include <AlgoVisual/Event.h>

class render_list {
	Event<void> renderables;
public:
	template <typename T>
	unsigned long add(T& renderer) {
		return renderables.add(std::move(std::bind(&T::render,&renderer)));
	}

	bool remove(unsigned long id) {
		return renderables.remove(id);
	}

	void render() {
		renderables.call();
	}
};