#include "OptionRenderers/OptionRenderer.h" // Include header file for base class
#include "global.h" // Include global variables
#include "imgui.h" // Include ImGUI library
#include "raylib.h" // Include Raylib library
#include "raymath.h" // Include Raymath library
#include "visual_container.h" // Include header file for visual container
#include <OptionRenderers/GraphRenderer.h> // Include header file for GraphRenderer class
#include <array> // Include header file for std::array
#include <vector> // Include header file for std::vector

namespace { // Anonymous namespace for internal linkage
    std::string _name = "Graph"; // Name of the graph renderer
}

GraphRenderer::GraphRenderer() : OptionRenderer() { // Constructor for GraphRenderer class
    createStarterList(); // Create a starting list for the graph
}


void GraphRenderer::update(double dt) {

    for (auto& value : numberGraph) {
        value.value.x = Lerp(value.value.x, value.value.targetX, INTERPOLATION_SPEED * dt);
        value.value.y = Lerp(value.value.y, value.value.targetY, INTERPOLATION_SPEED * dt);
    }

    if (!mouseDown) {
        if (IsMouseButtonPressed(0)) {
            mouseDown = true;
            mouseX = GetMouseX();
            mouseY = GetMouseY();

            Vector2 mouseWorldPos{};
            mouseWorldPos.x = GetMouseX();
            mouseWorldPos.y = GetMouseY();
            for (auto& node : numberGraph) {
                auto nodePos = transformPosition(node.value.x,node.value.y);
                if (Vector2Distance(mouseWorldPos, nodePos) <= (CIRCLE_SIZE / scale)) {
                    selectedNode = &node.value;
                    movingNode = true;
                    break;
                }
            }
        }
    }
    else {

        auto newMouseX = GetMouseX();
        auto newMouseY = GetMouseY();
         
        Vector2 mouseDelta{};
        mouseDelta.x = (newMouseX - mouseX) * scale;
        mouseDelta.y = (newMouseY - mouseY) * scale;

        if (movingNode && selectedNode != nullptr) {
            selectedNode->targetX += mouseDelta.x;
            selectedNode->targetY += mouseDelta.y;
        }

        mouseX = newMouseX;
        mouseY = newMouseY;

        if (IsMouseButtonReleased(0)) {
            mouseDown = false;
            movingNode = false;
        }
    }

    if (!movingNode) {
        OptionRenderer::update(dt);
    }
}

void GraphRenderer::render() {

    DrawBackground(2.0);
    //Draw lines firsts
    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {
        auto& node = i->value;
        auto circlePos = transformPosition(node.x, node.y);

        for (auto& connection : *i) {
            auto connectionPos = transformPosition(connection.value.x, connection.value.y);
            DrawLineEx(circlePos, connectionPos, 20 / scale, {232, 106, 23, 255});
            DrawLineEx(circlePos, connectionPos, 15 / scale, {250, 129, 50, 255});
        }
    }

    //Draw node circles
    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {
        auto& node = i->value;
        auto circlePos = transformPosition(node.x, node.y);

        if (selectedNode == &node) {
            DrawCircle(circlePos.x, circlePos.y, (CIRCLE_SIZE + 10) / scale, BLACK);
        }

        DrawTexture(getOrangeCircle(),circlePos,scale);

        auto str = std::to_string(node.value);

        float textWidth = MeasureText(str.c_str(), TEXT_SIZE);

        auto textPos = transformPosition(node.x - (textWidth / 2), node.y - (TEXT_SIZE / 2));

        DrawTextEx(GetFontDefault(), str.c_str(), textPos, TEXT_SIZE / scale, (TEXT_SIZE / scale) / 10.0, WHITE);
    }

    ImGui::InputFloat("Number",&selectedNumber);
    if (ImGui::Button("Add Number")) {
        push(selectedNumber);
    }

    if (ImGui::Button("Remove Number")) {
        pop(selectedNumber);
    }

    if (ImGui::Button("Remove Number")) {
        pop(selectedNumber);
    }

    if (ImGui::Button("Add Random Number")) {
        push((rand() % 2000) - 1000);
    }

    if (ImGui::Button("Clear")) {
        clear();
    }

    if (selectedNode != nullptr) {
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::InputFloat("Connection Number",&connectionNumber);
        if (ImGui::Button("Connect To Number")) {
        connect(selectedNumber);
    }

    if (ImGui::Button("Disconnect from Number")) {
        disconnect(selectedNumber);
    }
    }
}

// This function adds a new node with the specified value to the graph
void GraphRenderer::push(float value) {
    numberGraph.add_node(visual_container<float>(value));
}

// This function removes the node with the specified value from the graph
void GraphRenderer::pop(float value) {
    auto result = numberGraph.find_node_by([value](auto v){ return v.value == value;});

    if (result != numberGraph.end()) {
        numberGraph.delete_node(result);
        return;
    }
}

// This function clears the entire graph
void GraphRenderer::clear() {
    numberGraph.clear();
}

// This function connects the currently selected node to a node with the specified value
void GraphRenderer::connect(float value) {
    if (selectedNode == nullptr) {
        return;
    }

    auto sourceNode = numberGraph.find_node(*selectedNode);
    auto destination = numberGraph.find_node_by([&](auto v){ return v.value == connectionNumber; });

    if (destination != numberGraph.end()) {
        sourceNode->add_connection_to(destination);
    }
}

// This function disconnects the currently selected node from a node with the specified value
void GraphRenderer::disconnect(float value) {
    if (selectedNode == nullptr) {
        return;
    }

    auto sourceNode = numberGraph.find_node(*selectedNode);
    auto destination = numberGraph.find_node_by([&](auto v){ return v.value == connectionNumber; });

    if (destination != numberGraph.end()) {
        sourceNode->remove_connection_to(destination);
    }
}

// This function returns the name of the graph
const std::string& GraphRenderer::getName() const {
    return _name;
}

//Creates a starter list for the graph
void GraphRenderer::createStarterList() {
    constexpr std::array<float,11> numbersToAdd {5,1,2,11,15,0,4,3,9,2,13};

    int counter = 0;
    for (double angle = 0; angle <= 2 * PI; angle += (2.0 * PI) / numbersToAdd.size()) {
        if (counter >= numbersToAdd.size()) {
            break;
        }
        numberGraph.add_node(visual_container<float>(numbersToAdd[counter],cos(angle) * NODE_ANGLE_SIZE,sin(angle) * NODE_ANGLE_SIZE));

        counter++;
    }

    for (auto i = numberGraph.begin(); i != numberGraph.end(); i++) {
        for (auto j = numberGraph.begin(); j != numberGraph.end(); j++) {
            if (i == j) {
                continue;
            }
            if (rand() % 100 > 80) {
                i->add_connection_to(j);
            }
        }
    }
}

void GraphRenderer::onSave(std::ostream& outputStream) const {
    outputStream << 'g';
    outputStream << numberGraph.size();
    outputStream << 'g';

    std::vector<const visual_container<float>*> graph_nodes{};

    for (auto& node : numberGraph) {
        graph_nodes.push_back(&node.value);
        outputStream << node.value.value;
        outputStream << 'g';
        outputStream << node.value.targetX;
        outputStream << 'g';
        outputStream << node.value.targetY;
        outputStream << 'g';
    }

    for (auto& node : numberGraph) {
        outputStream << node.connections_size();
        outputStream << 'g';
        for (auto& connection : node) {
            for (int index = 0; index != graph_nodes.size(); index++) {
                if (graph_nodes[index] == &connection.value) {
                    outputStream << index;
                    outputStream << 'g';
                    break;
                }
            }
        }
    }
}

void GraphRenderer::onLoad(std::istream& inputStream) {
    char id;
    inputStream >> id;
    if (id != 'g') {
        throw std::exception();
    }
    numberGraph.clear();

    int size;
    inputStream >> size;
    inputStream >> id;
    std::vector<graph<visual_container<float>>::iterator> graph_nodes{};

    for (int i = 0; i < size; i++) {
        float value, targetX, targetY;
        inputStream >> value;
        inputStream >> id;
        inputStream >> targetX;
        inputStream >> id;
        inputStream >> targetY;
        inputStream >> id;

        auto node = numberGraph.add_node(visual_container<float>(value, targetX, targetY));
        graph_nodes.push_back(node);
    }

    for (int i = 0; i < graph_nodes.size(); i++) {
        int connections;
        inputStream >> connections;
        inputStream >> id;
        for (int c = 0; c < connections; c++) {
            int connectionIndex;
            inputStream >> connectionIndex;
            inputStream >> id;
            graph_nodes[i]->add_connection_to(graph_nodes[connectionIndex]);
        }
    }
}
