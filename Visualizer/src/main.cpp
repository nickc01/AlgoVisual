#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#include <test.h>
#include <raylib.h>
#include <linked_list.h>
#include <render_list.h>
#include <update_list.h>
#include <imgui.h>
#include <rlImGui.h>
#include <binary_search_tree.h>
#include <graph.h>
#include <options.h>

using namespace std;

constexpr const char* WINDOW_TITLE = "Algorithm Tester";

//TEST

std::vector<std::string> optionNames;
std::unique_ptr<const char* []> optionNamesCStr;
int itemCount = 0;

int selectedItem = 0;

void setup();
void cleanup();
void start_draw();
void end_draw();

int main()
{
	optionNames = create_option_strings();
	optionNamesCStr = convertToCharArrays(optionNames);
	//create_option_strings(&items, &itemCount);

	graph<int> testGraph;

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

	setup();

	while (!WindowShouldClose())
	{
		start_draw();

		bool open = true;

		ImGui::Begin(WINDOW_TITLE);

		ImGui::ListBox("TESTBOX2",&selectedItem, optionNamesCStr.get(), optionNames.size());

		ImGui::End();

		end_draw();
	}

	cleanup();
}

void start_draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	// start ImGui Conent
	rlImGuiBegin();
}

void end_draw()
{
	// end ImGui Content
	rlImGuiEnd();

	EndDrawing();
}

void setup()
{
	InitWindow(800, 600, WINDOW_TITLE);

	SetWindowState(FLAG_WINDOW_RESIZABLE);

	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	rlImGuiSetup(true);
}

void cleanup()
{
	CloseWindow();
}