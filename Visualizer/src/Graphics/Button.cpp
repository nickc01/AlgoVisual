#include <Graphics/Button.h>
#include <misc.h>

Button::Button(float posX, float posY, std::string text, int fontSize, int padding, Color color) :
	posX(posX),
	posY(posY),
	text(text),
	fontSize(fontSize),
	padding(padding),
	baseColor(color) {}

void Button::update(double dt)
{
	auto mouseX = GetMouseX();
	auto mouseY = GetMouseY();

	bool insideRect = PointWithinRect(calculate_rect(), mouseX, mouseY);

	if (hovered)
	{
		if (!insideRect)
		{
			hovered = false;
			OnUnhover.call();

			if (clicked)
			{
				clicked = false;
				OnRelease.call();
			}
		}
	}
	else
	{
		if (insideRect)
		{
			hovered = true;
			OnHover.call();
		}
	}

	if (hovered)
	{
		if (IsMouseButtonPressed(0))
		{
			clicked = true;
			OnClick.call();
		}

		if (IsMouseButtonReleased(0))
		{
			clicked = false;
			OnRelease.call();
		}
	}
}

void Button::render()
{
	auto textWidth = MeasureText(text.c_str(), fontSize);

	Rectangle rect;
	rect.x = posX + padding;
	rect.y = posY + padding;
	rect.width = textWidth + (padding * 2);
	rect.height = fontSize + (padding * 2);

	if (clicked)
	{
		DrawRectangle(rect.x, rect.y, rect.width, rect.height, clickColor);
	}
	else if (hovered)
	{
		DrawRectangle(rect.x, rect.y, rect.width, rect.height, hoverColor);
	}
	else
	{
		DrawRectangle(rect.x, rect.y, rect.width, rect.height, baseColor);
	}

	rect.x -= padding;
	rect.y -= padding;
	rect.width += (padding * 2);
	rect.height += (padding * 2);

	DrawRectangleLinesEx(rect, padding, BLACK);
	DrawText(text.c_str(), posX + (padding * 2), posY + (padding * 2), fontSize, WHITE);
}

Rectangle Button::calculate_rect()
{
	auto textWidth = MeasureText(text.c_str(), fontSize);

	Rectangle rect;
	rect.x = posX;
	rect.y = posY;
	rect.width = textWidth + (padding * 4);
	rect.height = fontSize + (padding * 4);
	return rect;
}

