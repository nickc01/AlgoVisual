#pragma once
#include "OptionRenderers/OptionRenderer.h"
#include "binary_search_tree.h"
#include "visual_container.h"

class BinarySearchTreeRenderer : public OptionRenderer {
    binary_search_tree<visual_container<float>> tree {[](const visual_container<float>& a, const visual_container<float>& b){
        return a.value < b.value;
    }};

    struct positionResult {
        float leftWidth = 0;
        float rightWidth = 0;

        constexpr positionResult(float _l, float _r) : leftWidth(_l), rightWidth(_r) {}
        constexpr positionResult() {};
    };

    void renderNode(decltype(tree)::iterator node);
    void renderNodeLines(decltype(tree)::iterator node);
    void updateNodes(decltype(tree)::iterator node, double dt);
    void createStarterList();

    positionResult updateNodePositions(decltype(tree)::iterator node);
    void moveNodeAndChildren(decltype(tree)::iterator node, float diffX, float diffY);
    void setNodeAndChildrenPos(decltype(tree)::iterator node, float newX, float newY);

    float selectedNumber = 0.0;
    bool selfBalancing = true;


    bool add_number(float number);
    bool remove_number(float number);
    bool add_random_number();
    void clear();
protected:
    void onSave(std::ostream& outputStream) const override;
    void onLoad(std::istream& inputStream) override;

    void onLoad(decltype(tree)::iterator node, std::istream& inputStream);
    void onSave(decltype(tree)::const_iterator root, std::ostream& outputStream) const;
public:
    void update(double dt) override;

    void render() override;

    const std::string& getName() const override;

    BinarySearchTreeRenderer();
};