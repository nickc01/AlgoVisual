#include "OptionRenderers/OptionRenderer.h"
#include "binary_search_tree.h"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include "visual_container.h"
#include <OptionRenderers/BinarySearchTreeRenderer.h>

namespace {
    std::string _name = "Binary Search Tree";
}

BinarySearchTreeRenderer::BinarySearchTreeRenderer() : OptionRenderer() {
    createStarterList();
}

void BinarySearchTreeRenderer::update(double dt) {
    OptionRenderer::update(dt);

    updateNodes(tree.getRoot(), dt);
}

void BinarySearchTreeRenderer::updateNodes(decltype(tree)::iterator node, double dt) {
    if (node == tree.end()) {
        return;
    }

    node->x = Lerp(node->x, node->targetX, INTERPOLATION_SPEED * dt);
    node->y = Lerp(node->y, node->targetY, INTERPOLATION_SPEED * dt);

    updateNodes(node.getLeft(), dt);
    updateNodes(node.getRight(), dt);
}

void BinarySearchTreeRenderer::render() {
    renderNodeLines(tree.getRoot());
    renderNode(tree.getRoot());
    ImGui::Spacing();
    ImGui::TextWrapped("A binary search tree is simply a collection of nodes. Each node can only have up to two children, a left child and a right child. The left child will always have a number value less than it's parent, while the right child will always have a number greater than its parent. This structure allows for very fast searching. No duplicate values are allowed in a tree");
    ImGui::Spacing();
    ImGui::InputFloat("Number",&selectedNumber);

    if (ImGui::Button("Add Number"))
    {
        add_number(selectedNumber);
    }

    if (ImGui::Button("Remove Number"))
    {
        remove_number(selectedNumber);
    }

    if (ImGui::Button("Add Random Number"))
    {
        add_random_number();
    }

    if (selfBalancing) {
        if (ImGui::Button("Disable Self Balancing")) {
            selfBalancing = false;
            tree.setSelfBalancing(false);
        }
    }
    else {
        if (ImGui::Button("Enable Self Balancing")) {
            selfBalancing = true;
            tree.setSelfBalancing(true);
        }
    }

    if (ImGui::Button("Clear")) {
        clear();
    }
}

void BinarySearchTreeRenderer::renderNodeLines(decltype(tree)::iterator node) {
    if (node == tree.end()) {
        return;
    }
    auto nodePos = transformPosition(node->x, node->y);

    if (node.getParent() != tree.end()) {
        auto parent = node.getParent();
        auto parentPos = transformPosition(parent->x, parent->y);
        DrawLineEx(nodePos, parentPos, 20 / scale, ORANGE);
    }

    renderNodeLines(node.getLeft());
    renderNodeLines(node.getRight());
}

void BinarySearchTreeRenderer::renderNode(decltype(tree)::iterator node) {
    if (node == tree.end()) {
        return;
    }
    auto circlePos = transformPosition(node->x, node->y);

    DrawCircle(circlePos.x,circlePos.y,CIRCLE_SIZE / scale,RED);

    auto str = std::to_string(node->value);

    float textWidth = MeasureText(str.c_str(),TEXT_SIZE);

    auto textPos = transformPosition(node->x  - (textWidth / 2), node->y - (TEXT_SIZE / 2));

    DrawTextEx(GetFontDefault(), str.c_str(),textPos, TEXT_SIZE / scale, (TEXT_SIZE / scale) / 10.0, WHITE);

    renderNode(node.getLeft());
    renderNode(node.getRight());
}

const std::string& BinarySearchTreeRenderer::getName() const {
    return _name;
}

void BinarySearchTreeRenderer::createStarterList() {
    tree.insert(visual_container<float>(5));
    tree.insert(visual_container<float>(1));
    tree.insert(visual_container<float>(2));
    tree.insert(visual_container<float>(11));
    tree.insert(visual_container<float>(15));
    tree.insert(visual_container<float>(0));
    tree.insert(visual_container<float>(4));
    tree.insert(visual_container<float>(3));
    tree.insert(visual_container<float>(9));
    tree.insert(visual_container<float>(20));
    tree.insert(visual_container<float>(13));

    updateNodePositions(tree.getRoot());
}

constexpr inline int Max(int a, int b) {
    return a > b ? a : b;
}

void BinarySearchTreeRenderer::setNodeAndChildrenPos(decltype(tree)::iterator node, float newX, float newY) {
    if (node == tree.end()) {
        return;
    }
    int diffX = newX - node->targetX;
    int diffY = newY - node->targetY;
    moveNodeAndChildren(node, diffX, diffY);
}

void BinarySearchTreeRenderer::moveNodeAndChildren(decltype(tree)::iterator node, float diffX, float diffY) {
    if (node == tree.end()) {
        return;
    }
    node->targetX += diffX;
    node->targetY += diffY;
    moveNodeAndChildren(node.getLeft(), diffX, diffY);
    moveNodeAndChildren(node.getRight(), diffX, diffY);
}

BinarySearchTreeRenderer::positionResult BinarySearchTreeRenderer::updateNodePositions(decltype(tree)::iterator node) {
    if (node == tree.end()) {
        return BinarySearchTreeRenderer::positionResult{};
    }

    auto leftResult = updateNodePositions(node.getLeft());
    auto rightResult = updateNodePositions(node.getRight());

    if (node.getParent() == tree.end()) {
        node->targetX = 0;
        node->targetY = 0;
    }
    else if (node.getLeft() == tree.end() && node.getRight() == tree.end()) {
        return positionResult{0,0};
    }
    setNodeAndChildrenPos(node.getLeft(), node->targetX - leftResult.rightWidth - (CIRCLE_SPACING / 2), node->targetY + CIRCLE_SPACING);
    setNodeAndChildrenPos(node.getRight(), node->targetX + rightResult.leftWidth + (CIRCLE_SPACING / 2), node->targetY + CIRCLE_SPACING);

    float leftWidth = leftResult.leftWidth;
    float rightWidth = rightResult.rightWidth;

    if (node.getLeft() != tree.end()) {
        leftWidth += leftResult.rightWidth + (CIRCLE_SPACING / 2);
    } 

    if (node.getRight() != tree.end()) {
        rightWidth += rightResult.leftWidth + (CIRCLE_SPACING / 2);
    }


    return positionResult{leftWidth,rightWidth};
}

bool BinarySearchTreeRenderer::add_number(float number) {
    auto node = tree.insert(visual_container<float>(number));
    if (node != tree.end()) {
        updateNodePositions(tree.getRoot());
        return true;
    }
    return false;
}

bool BinarySearchTreeRenderer::remove_number(float number) {
    auto result = tree.find(visual_container<float>(number));
    if (result != tree.end()) {
        tree.remove(result);
        updateNodePositions(tree.getRoot());
        return true;
    }
    return false;
}

bool BinarySearchTreeRenderer::add_random_number() {
    for (int i = 0; i < 1000; i++) {
        if (add_number((rand() % 2000) - 1000)) {
            return true;
        }
    }
    return false;
}

void BinarySearchTreeRenderer::clear() {
    tree.clear();
}
