#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#include <AlgoVisual/test.h>
#include <raylib.h>
#include <AlgoVisual/DataStructures/linked_list.h>
#include <AlgoVisual/render_list.h>
#include <AlgoVisual/update_list.h>
#include <imgui.h>
#include <AlgoVisual/rlImGui.h>
#include <AlgoVisual/DataStructures/binary_search_tree.h>

using namespace std;

int main()
{
	binary_search_tree<int> testTree;

	linked_list<int> test = { 1,2,3,4,5,6,7,8,9,10 };

	test.pop_front();
	test.pop_back();

	auto iter = test.front();

	for (int i = 0; i < 5; i++)
	{
		iter++;
	}

	test.insert(12345, iter);
	test.pop_element(iter);

	std::cout << test << std::endl;

	InitWindow(800, 600, "Test Window");

	SetWindowState(FLAG_WINDOW_RESIZABLE);

	SetTargetFPS(60);

	//Button button{ 400, 300, "This is a test" };

	//render_list renderables;
	//update_list updatables;

	//renderables.add(button);
	//updatables.add(button);

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		auto dt = GetFrameTime();

		//updatables.update(dt);

		BeginDrawing();

		ClearBackground(RAYWHITE);

		stringstream stream;

		stream << test;

		auto listText = stream.str();

		constexpr int font_size = 40;

		auto size = MeasureText(listText.c_str(), font_size);

		auto windowHeight = GetScreenHeight();
		auto windowWidth = GetScreenWidth();


		//renderables.render();

		DrawText(listText.c_str(), (windowWidth / 2) - (size / 2), (windowHeight / 2) - (font_size / 2), font_size, RED);

		// start ImGui Conent
		rlImGuiBegin();

		// show ImGui Content
		bool open = true;
		//ImGui::ShowDemoWindow(&open);

		ImGui::Begin("Test Window");

		ImGui::End();

		// end ImGui Content
		rlImGuiEnd();

		EndDrawing();
	}

	CloseWindow();
}