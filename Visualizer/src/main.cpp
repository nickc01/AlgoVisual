#include "OptionRenderers/AlgorithmRenderer.h"
#include "common.h"
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
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
#include <OptionRenderers/OptionRenderer.h>
#include <quick_sort.h>

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

std::vector<std::shared_ptr<OptionRenderer>> renderers;

void createRenderers();


int main()
{
	optionNames = create_option_strings();
	optionNamesCStr = convertToCharArrays(optionNames);

	update_list updatables;
	render_list renderables;




	//create_option_strings(&items, &itemCount);

	/*graph<int> testGraph;

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

	std::cout << test << std::endl;*/

	setup();

	while (!WindowShouldClose())
	{
		start_draw();

		ImGui::Begin(WINDOW_TITLE);

		auto original = selectedItem;
		ImGui::ListBox("Options",&selectedItem, optionNamesCStr.get(), optionNames.size());

		if (uiLocked()) {
			selectedItem = original;
		}



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

void createRenderers() {
	using arrayType = std::vector<float>;

	using sorterType = std::function<void(arrayType&)>;
	//QuickSort
	/*auto renderer = AlgorithmRenderer{[](std::vector<float>& list){
		quick_sort<std::vector<float>>(list);
	}};
	renderers.push_back(std::move(renderer));*/

	arrayType test;

	//using testType = typename std::decay<decltype(*std::begin(test))>::type;

	//auto& ref = test;

	quick_sort(test);
	/*auto sorter = [](arrayType& list){
		quick_sort(list);
	};*/

	//using testType2 = decltype(sorting_impl::DefaultComparer<decltype(*std::begin(test))>);

	//AlgorithmRenderer tester {std::move(sorter)};

	//auto renderer = std::make_shared<AlgorithmRenderer>(std::move(sorter));

	//renderers.emplace_back(std::move(renderer));

	//quick_sort(test);

	/*auto renderer = std::make_shared<AlgorithmRenderer>([](arrayType& list){
		quick_sort(std::begin(list),std::end(list));
	});
	renderers.emplace_back(std::move(renderer));*/
}

void cleanup()
{
	CloseWindow();
}