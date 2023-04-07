#include "OptionRenderers/BinarySearchTreeRenderer.h"
#include "OptionRenderers/OptionRenderer.h"
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
#include <OptionRenderers/AlgorithmRenderer.h>
#include <quick_sort.h>
#include <insertion_sort.h>
#include <bubble_sort.h>
#include <global.h>

using namespace std;

constexpr const char* WINDOW_TITLE = "Structure and Algorithm Tester";

//TEST

//std::vector<std::string> optionNames;
std::unique_ptr<const char* []> optionNamesCStr;
int itemCount = 0;

int selectedItem = 0;

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
	createRenderers();
	//optionNames = create_option_strings();
	optionNamesCStr = convertToCharArrays(optionRenderers);

	selectedRenderer = optionRenderers[selectedItem];

	/*update_list updatables;
	render_list renderables;

	for (auto renderer : optionRenderers) {
		updatables.add(*renderer);
		renderables.add(*renderer);
	}*/




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
		ImGui::ListBox("Options",&selectedItem, optionNamesCStr.get(), optionRenderers.size());

		if (uiLocked()) {
			selectedItem = original;
		}
		
		if (selectedRenderer != optionRenderers[selectedItem]) {
			selectedRenderer = optionRenderers[selectedItem];
		}

		selectedRenderer->update(GetFrameTime());
		selectedRenderer->render();



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
	//using arrayType = linked_list<float>;

	//linked_list<AlgorithmRenderer::valueContainer> test;

	//using sorterType = std::function<void(arrayType&)>;
	//QuickSort
	/*auto renderer = AlgorithmRenderer{[](std::vector<float>& list){
		quick_sort<std::vector<float>>(list);
	}};
	renderers.push_back(std::move(renderer));*/

	//arrayType test;

	//using testType = typename std::decay<decltype(*std::begin(test))>::type;

	//auto& ref = test;

	//quick_sort(test);
	/*auto sorter = [](arrayType& list) {
		quick_sort(list);
	};*/

	//using testType2 = decltype(sorting_impl::DefaultComparer<decltype(*std::begin(test))>);

	//AlgorithmRenderer tester {std::move(sorter)};

	//auto renderer = std::make_shared<AlgorithmRenderer>(std::move(sorter));

	//renderers.emplace_back(std::move(renderer));

	//quick_sort(test);

	//auto renderer = ;

	//auto result = std::function<decltype(testFunc)>{testFunc};

	//auto final = result(123);

	/*optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Quick Sort", [](arrayType& list) {
		quick_sort_list(list);
		}));*/

	std::string quick_sort_desc = "Quick Sort works by first picking a random value in the list to serve as a pivot. Then, the list is rearranged into two sub-sections. The left side of the array will contain values less than the pivot, while the right side will have values greater than the pivot. The pivot value sits in-between the two sides, seperating the two subsections from each other. After this, the same process is repeated on each sub-section, creating sub-sub-sections. This process continues untill there are no sub-sections to rearrange, which means the array is fully sorted";
	std::string insertion_sort_desc = "Insertion Sort works by iterating through each element in the list, and inserting it at it's leftmost point in the list. It does this by continously swapping an element with the previous element AS LONG AS the previous element is bigger. When it finds a previous element that is equal or smaller, it stops swapping and continues onto the next element. It does this for every element in the list, and the end result is a sorted list";
	std::string bubble_sort_desc = "Bubble sort is a simple algorithm where it will look over all the elements and find if an element is less than it's previous element. If it finds an element that's less than it's previous one, it will do a swap and move on to the next element. When it reaches the end of the list, it will go back to the beginning and do the test all over again. This continues until it finds no elements that are less than their previous, which means the list is sorted";

	optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Quick Sort", std::move(quick_sort_desc), ALGORITHM(quick_sort)));
	optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Insertion Sort", std::move(insertion_sort_desc), ALGORITHM(insertion_sort)));
	optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Bubble Sort", std::move(bubble_sort_desc), ALGORITHM(bubble_sort)));
	optionRenderers.emplace_back(std::make_shared<BinarySearchTreeRenderer>());

	//auto f = ALGORITHM(quick_sort);

	//auto comparer = std::function<decltype(AlgorithmRenderer::visualValueComparer)>{AlgorithmRenderer::visualValueComparer};

	//f(test.begin(),test.end(),std::function<decltype(AlgorithmRenderer::visualValueComparer)>(AlgorithmRenderer::visualValueComparer),[](AlgorithmRenderer::iterType& a, AlgorithmRenderer::iterType& b){});

	//auto testRend = dynamic_cast<AlgorithmRenderer*>(optionRenderers[0].get());

	//testRend->algorithmFunc(test.begin(),test.end(),std::function<decltype(AlgorithmRenderer::visualValueComparer)>(AlgorithmRenderer::visualValueComparer),[](AlgorithmRenderer::iterType& a, AlgorithmRenderer::iterType& b){});
	//auto testVar = std::function<decltype(ALGORITHM(quick_sort))>{ALGORITHM(quick_sort)};

	//testVar(test.begin(),test.end(),sorting_impl::DefaultComparer<AlgorithmRenderer::valueContainer>,sorting_impl::DefaultSwapper<decltype(test.begin())>);

	/*optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Insertion Sort", [](arrayType& list) {
		insertion_sort_list(list);
		}));

	optionRenderers.emplace_back(std::make_shared<AlgorithmRenderer>("Bubble Sort", [](arrayType& list) {
		bubble_sort_list(list);
		}));*/
}

void cleanup()
{
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