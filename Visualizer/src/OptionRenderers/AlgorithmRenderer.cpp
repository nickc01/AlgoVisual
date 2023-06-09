#include "OptionRenderers/OptionRenderer.h" // Include for the parent class
#include "common.h" // Common header file
#include "global.h" // Global header file
#include "imgui.h" // Dear ImGui library
#include "raylib.h" // RayLib library
#include "raymath.h" // RayMath library
#include <OptionRenderers/AlgorithmRenderer.h> // Include for AlgorithmRenderer class
#include <chrono> // Chrono library for time manipulation
#include <functional> // Functional library
#include <memory> // Memory library for smart pointers
#include <string> // String library
#include <thread> // Thread library for concurrency

void AlgorithmRenderer::update(double dt) {
    OptionRenderer::update(dt); // Call parent class update method
    for (auto& value : numberList) {
        // Use linear interpolation to update the position of each number
        value.x = Lerp(value.x, value.targetX, INTERPOLATION_SPEED * dt);
        value.y = Lerp(value.y, value.targetY, INTERPOLATION_SPEED * dt);
    }
}

void AlgorithmRenderer::render() {
    DrawBackground(2.0); // Draw the background with a scale factor of 2.0
    ImGui::TextWrapped("%s", _description.c_str()); // Display the description of the algorithm
    if (!uiLocked()) { // Check if the UI is not locked
        ImGui::InputFloat("Number",&selectedNumber); // Display an input box for entering a number
        if (ImGui::Button("Add Number To Start")) { // Add a button for adding the entered number to the start of the list
            push_front(selectedNumber);
        }

        if (ImGui::Button("Add Number To End")) { // Add a button for adding the entered number to the end of the list
            push_back(selectedNumber);
        }

        if (ImGui::Button("Remove Number")) { // Add a button for removing the entered number from the list
            pop(selectedNumber);
        }

        if (ImGui::Button("Remove Number from end")) { // Add a button for removing the last number from the list
            pop_back();
        }

        if (ImGui::Button("Remove Number from front")) { // Add a button for removing the first number from the list
            pop_front();
        }

        if (ImGui::Button("Add Random Number")) { // Add a button for adding a random number to the list
            push_back((rand() % 2000) - 1000);
        }

        if (ImGui::Button("Shuffle")) { // Add a button for shuffling the list
            shuffle();
        }

        if (ImGui::Button("Clear")) { // Add a button for clearing the list
            clear();
        }

        if (ImGui::Button("Sort")) { // Add a button for sorting the list
            beginSort([&](){
                #ifdef __cpp_lib_bind_front
                    algorithmFunc(numberList.begin(),numberList.end(),comparerType(visualValueComparer),std::bind_front(&AlgorithmRenderer::swap, *this));
                #else
                    algorithmFunc(numberList.begin(),numberList.end(),comparerType(visualValueComparer),std::bind(&AlgorithmRenderer::swap,*this, std::placeholders::_1,std::placeholders::_2));
                #endif
            });
        }

        ImGui::Spacing(); // Add some spacing between the buttons
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();


        ImGui::InputFloat("Before Number",&beforeNumber);

        if (ImGui::Button("Insert Number Before Number")) {
            for (auto node = numberList.begin(); node != numberList.end(); node++) {
                if (node->value == beforeNumber) {
                    auto newNode = numberList.insert(selectedNumber,node);
                    if (newNode == numberList.begin()) {
                        auto next = ++numberList.begin();
                        newNode->x = next->targetX;
                        newNode->targetX = next->targetX - CIRCLE_SPACING;
                        newNode->targetY = next->targetY;
                    }
                    else {
                        auto previous = newNode;
                        --previous;
                        newNode->x = previous->targetX;
                        newNode->targetX = previous->targetX + CIRCLE_SPACING;
                        newNode->targetY = previous->targetY;
                        for (auto j = ++newNode; j != numberList.end(); j++) {
                            j->targetX += CIRCLE_SPACING;
                        }
                    }
                    break;
                }
            }
        }

    }
    else {
        if (ImGui::Button("Stop Sorting")) {
            endSort();
        }
    }

    for (auto value : numberList) {

        auto circlePos = transformPosition(value.x, value.y);

        DrawTexture(getOrangeCircle(),circlePos,scale);

        auto str = std::to_string(value.value);

        float textWidth = MeasureText(str.c_str(),TEXT_SIZE);

        auto textPos = transformPosition(value.x  - (textWidth / 2), value.y - (TEXT_SIZE / 2));

        DrawTextEx(GetFontDefault(), str.c_str(),textPos, TEXT_SIZE / scale, (TEXT_SIZE / scale) / 10.0, WHITE);
    }
}

// Gets the name of the algorithm
const std::string& AlgorithmRenderer::getName() const {
    return _name;
}

// Swaps two values with each other. Used for visualizing swaps
void AlgorithmRenderer::swap(decltype(numberList.begin())& a, decltype(numberList.begin())& b) {
    // If the sorting is stopping, throw an exception to stop the process.
    if (stoppingSort()) {
        throw std::exception();
    }
    // Store the value of 'a' in a temporary variable.
    visual_container<float> temp = *a;
    // Assign 'b' to 'a'.
    a->value = b->value;
    a->x = b->x;
    a->y = b->y;
    // Assign the temporary variable to 'b'.
    b->value = temp.value;
    b->x = temp.x;
    b->y = temp.y;
    // Wait for 500 milliseconds.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

// Creates a starting list
void AlgorithmRenderer::createStarterList() {
    // Add visual containers to the linked_list, representing the starting list.
    numberList.push_back(visual_container<float>(5,-5 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(1,-4 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(2,-3 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(11,-2 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(15,-1 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(0,0 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(4,1 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(3,2 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(9,3 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(2,4 * CIRCLE_SPACING,0));
    numberList.push_back(visual_container<float>(13,5 * CIRCLE_SPACING,0));
}

// Adds a new value to the list
void AlgorithmRenderer::push_front(float value) {
    auto oldFront = numberList.front();
    // Set the target X and Y for the new value.
    float targetX = oldFront != numberList.end() ? oldFront->targetX : CIRCLE_SPACING;
    float targetY = oldFront != numberList.end() ? oldFront->targetY : 0;
    visual_container<float> newValue = visual_container<float> {value,targetX,targetY};
    // Shift the target X of all values to the right.
    newValue.targetX -= CIRCLE_SPACING;
    // Add the new value to the front of the list.
    numberList.push_front(newValue);
}

//Adds a new value to the end of the list
void AlgorithmRenderer::push_back(float value) {
    auto oldBack = numberList.back();

    // Determine the target position of the new element
    float targetX = oldBack != numberList.end() ? oldBack->targetX : -CIRCLE_SPACING;
    float targetY = oldBack != numberList.end() ? oldBack->targetY : 0;

    // Create a new visual_container object with the given value and target position
    visual_container<float> newValue = visual_container<float> {value,targetX,targetY};
    newValue.targetX += CIRCLE_SPACING;

    // Add the new element to the end of the list
    numberList.push_back(std::move(newValue));
}

//Removes the first element from the list
void AlgorithmRenderer::pop_front() {
    numberList.pop_front();
}

//Removes the last element from the list
void AlgorithmRenderer::pop_back() {
    numberList.pop_back();
}

//Removes the element with the given value from the list
void AlgorithmRenderer::pop(float value) {
    for (auto i = numberList.begin(); i != numberList.end(); i++)
    {
        if (i->value == value)
        {
            // Shift the target positions of all elements after the removed element
            for (auto j = i; j != numberList.end(); j++) {
                if (j == i) {
                    continue;
                }
                j->targetX -= CIRCLE_SPACING;
            }

            // Remove the element from the list
            numberList.pop_element(i);
            break;
        }
    }
}

//Removes all elements from the list
void AlgorithmRenderer::clear() {
    // Reset camera position
    dest_cam_x = 0;
    dest_cam_y = 0;
    cam_x = 0;
    cam_y = 0;

    // Remove all elements from the list
    numberList.clear();
}

//Shuffles the elements in the list
void AlgorithmRenderer::shuffle() {
    for (int times = 0; times < numberList.getSize(); times++) {
        auto begin = numberList.begin();
        for (int i = 0; i < numberList.getSize(); i++) {
            auto destination = numberList.begin();
            for (int j = (rand() % numberList.getSize()); j > 0; j--) {
                ++destination;
            }

            // Swap the values and positions of the two elements
            visual_container<float> temp = *begin;
            begin->value = destination->value;
            begin->x = destination->x;
            begin->y = destination->y;

            destination->value = temp.value;
            destination->x = temp.x;
            destination->y = temp.y;
        }
    }
}

void AlgorithmRenderer::onSave(std::ostream& outputStream) const {
    outputStream << 'a';
    outputStream << (size_t)_name.length();
    outputStream << _name;
    outputStream << (size_t)_description.length();
    outputStream << _description;
    outputStream << numberList.getSize();
    outputStream << 'a';
    for (auto& n : numberList) {
        outputStream << n.value;
        outputStream << 'a';
        outputStream << n.targetX;
        outputStream << 'a';
        outputStream << n.targetY;
        outputStream << 'a';
    }
    outputStream.flush();
}

void AlgorithmRenderer::onLoad(std::istream& inputStream) {
    char id;
    inputStream >> id;
    if (id != 'a') {
        throw std::exception();
    }
    numberList.clear();
    size_t nameSize = 0;
    inputStream >> nameSize;
    inputStream.read(getBuffer().get(),nameSize);
    _name = std::string{getBuffer().get(),(size_t)nameSize};

    size_t descSize = 0;
    inputStream >> descSize;
    inputStream.read(getBuffer().get(),descSize);
    _description = std::string{getBuffer().get(),(size_t)descSize};

    int numberSize = 0;
    inputStream >> numberSize;
    inputStream >> id;

    for (int i = 0; i < numberSize; i++) {
        decltype(numberList.begin()->value) val;
        inputStream >> val;
        inputStream >> id;

        visual_container<float> container {val};

        inputStream >> container.targetX;
        inputStream >> id;
        inputStream >> container.targetY;
        inputStream >> id;
        
        container.x = container.targetX;
        container.y = container.targetY;

        numberList.push_back(std::move(container));
    }
}