#include "OptionRenderers/BinarySearchTreeRenderer.h"
#include "OptionRenderers/OptionRenderer.h"
#include "common.h"
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <functional>

#include <raylib.h>
#include <linked_list.h>
#include <imgui.h>
#include <rlImGui.h>
#include <binary_search_tree.h>
#include <graph.h>
#include <OptionRenderers/AlgorithmRenderer.h>
#include <quick_sort.h>
#include <insertion_sort.h>
#include <bubble_sort.h>
#include <global.h>
#include <OptionRenderers/GraphRenderer.h>
#include <nfd.h>

using namespace std;

constexpr const char* WINDOW_TITLE = "Structure and Algorithm Tester";
std::unique_ptr<const char* []> optionNamesCStr;
int itemCount = 0;
int selectedItem = 0;
std::string errorMessage;

void setup();
void cleanup();
void start_draw();
void end_draw();

std::vector<std::shared_ptr<OptionRenderer>> optionRenderers;
void createRenderers();
std::unique_ptr<const char*[]> convertToCharArrays(const decltype(optionRenderers)& values);

std::shared_ptr<OptionRenderer> selectedRenderer = nullptr;


int main()
{
	NFD_Init();
	createRenderers();
	optionNamesCStr = convertToCharArrays(optionRenderers);

	selectedRenderer = optionRenderers[selectedItem];

	setup();

	while (!WindowShouldClose())
	{
		start_draw();

		ImGui::Begin(WINDOW_TITLE);

		auto original = selectedItem;
		ImGui::ListBox("Options",&selectedItem, optionNamesCStr.get(), optionRenderers.size());

		if (uiLocked()) {
			selectedItem = original;
		}
		
		if (selectedRenderer != optionRenderers[selectedItem]) {
			selectedRenderer = optionRenderers[selectedItem];
		}

		selectedRenderer->update(GetFrameTime());
		selectedRenderer->render();

		if (!uiLocked()) {
			if(ImGui::Button("Save To File")) {
				try {
					selectedRenderer->save();
					errorMessage = "";
				} catch (...) {
					errorMessage = "Failed to save " + selectedRenderer->getName() + " to a file";
				}
				
			}

			if (ImGui::Button("Load from File")) {
				try {
					selectedRenderer->load();
				} catch (...) {
					errorMessage = "Failed to load file for " + selectedRenderer->getName();
				}
			}
		}

		ImGui::Spacing();

		ImGui::End();

		end_draw();
	}

	cleanup();

	NFD_Quit();
}

void start_draw()
{
	BeginDrawing();

	ClearBackground(GRAY);

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

	loadTextures();
}

void createRenderers() {
	std::string quick_sort_desc = "Quick Sort works by first picking a random value in the list to serve as a pivot. Then, the list is rearranged into two sub-sections. The left side of the array will contain values less than the pivot, while the right side will have values greater than the pivot. The pivot value sits in-between the two sides, seperating the two subsections from each other. After this, the same process is repeated on each sub-section, creating sub-sub-sections. This process continues untill there are no sub-sections to rearrange, which means the array is fully sorted";
	std::string insertion_sort_desc = "Insertion Sort works by iterating through each element in the list, and inserting it at it's leftmost point in the list. It does this by continously swapping an element with the previous element AS LONG AS the previous element is bigger. When it finds a previous element that is equal or smaller, it stops swapping and continues onto the next element. It does this for every element in the list, and the end result is a sorted list";
	std::string bubble_sort_desc = "Bubble sort is a simple algorithm where it will look over all the elements and find if an element is less than it's previous element. If it finds an element that's less than it's previous one, it will do a swap and move on to the next element. When it reaches the end of the list, it will go back to the beginning and do the test all over again. This continues until it finds no elements that are less than their previous, which means the list is sorted";

	optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Quick Sort", std::move(quick_sort_desc), ALGORITHM(quick_sort)));
	optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Insertion Sort", std::move(insertion_sort_desc), ALGORITHM(insertion_sort)));
	optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Bubble Sort", std::move(bubble_sort_desc), ALGORITHM(bubble_sort)));
	optionRenderers.emplace_back(std::make_shared<BinarySearchTreeRenderer>());
	optionRenderers.emplace_back(std::make_shared<GraphRenderer>());
}

void cleanup()
{
	unloadTextures();

	CloseWindow();

	endSort();
	auto& sortThread = getSortThread();
	if (sortThread.joinable()) {
		sortThread.join();
	}
}

std::unique_ptr<const char*[]> convertToCharArrays(const decltype(optionRenderers)& values)
{
	auto ptr = std::unique_ptr<const char* []>(new const char*[values.size()]);

	for (int i = 0; i < values.size(); i++)
	{
		ptr[i] = values[i]->getName().c_str();
	}
	return ptr;
}