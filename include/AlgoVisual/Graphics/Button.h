#pragma once
#include <AlgoVisual/Event.h>
#include <raylib.h>
#include <string>

class Button {
	bool hovered = false;
	bool clicked = false;
public:
	Event<void> OnHover;
	Event<void> OnUnhover;
	Event<void> OnClick;
	Event<void> OnRelease;

	float posX;
	float posY;
	std::string text;
	int fontSize = 20;
	int padding = 5;
	Color baseColor = RED;
	Color hoverColor = BLUE;
	Color clickColor = GREEN;

	Button(float posX, float posY, std::string text, int fontSize = 20, int padding = 5, Color color = RED);

	void update(double dt);

	void render();

	Rectangle calculate_rect();
};