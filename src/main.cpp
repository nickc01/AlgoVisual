#include <iostream>
#include <sstream>
#include <string>

#include <AlgoVisual/test.h>
#include <raylib.h>
#include <AlgoVisual/DataStructures/linked_list.h>
#include <imgui.h>
#include <rlImGui.h>

using namespace std;

int main() 
{
	linked_list<int> test = {1,2,3,4,5,6,7,8,9,10};

	test.removeBegin();
	test.removeEnd();

	std::cout << test << std::endl;

	InitWindow(800, 600, "Test Window");

	SetWindowState(FLAG_WINDOW_RESIZABLE);

	SetTargetFPS(60);

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		stringstream stream;

		stream << test;

		auto listText = stream.str();

		constexpr int font_size = 40;

		auto size = MeasureText(listText.c_str(), font_size);

		auto windowHeight = GetScreenHeight();
		auto windowWidth = GetScreenWidth();




		DrawText(listText.c_str(), (windowWidth / 2) - (size / 2), (windowHeight / 2) - (font_size / 2), font_size, RED);

		// start ImGui Conent
		rlImGuiBegin();

		// show ImGui Content
		bool open = true;
		ImGui::ShowDemoWindow(&open);

		// end ImGui Content
		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();
}