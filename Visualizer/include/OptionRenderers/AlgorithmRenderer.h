//This file contains the header file for the Algorithm Renderer. Used for rendering algorithms
#pragma once

//Required headers
#include "quick_sort.h"
#include "visual_container.h"
#include <OptionRenderers/OptionRenderer.h>
#include <functional>
#include <linked_list.h>

// The following macro is used to define a template function that can take
// different types of algorithms as arguments.
#define ALGORITHM(FUNC)                                                        \
  FUNC<AlgorithmRenderer::iterType, AlgorithmRenderer::comparerType>

//Used for rendering sorting algorithms
class AlgorithmRenderer : public OptionRenderer {
public:
    //Used for comparing two visual_containers with each other
    static constexpr bool visualValueComparer(const visual_container<float> &a,
                                                const visual_container<float> &b) {
        return a.value < b.value;
    }

private:
    //A linked_list of numbers to display
    linked_list<visual_container<float>> numberList;
    //The name of the algorithm being used
    std::string _name;
    //A description of the algorithm being used
    std::string _description;
    //The selected number to be added/removed
    float selectedNumber = 0.0;
    //The number thats used for inserting a new number after it
    float beforeNumber = 0.0;

    //Adds a new value to the list
    void push_front(float value);

    //Removes a value from the list
    void push_back(float value);

    //Removes the first number
    void pop_front();

    //Removes the last number
    void pop_back();

    //Removes the specified value from the list
    void pop(float value);

    //Clears the entire list
    void clear();

    //Shuffles the entire list
    void shuffle();

    //Creates a starting list
    void createStarterList();

    //Swaps two values with each other. Used for visualizing swaps
    void swap(decltype(numberList.begin()) &a, decltype(numberList.begin()) &b);

protected:
    //Called when the list is being saved to a file
    void onSave(std::ostream &outputStream) const override;

    //Called when the list is being loaded from a file
    void onLoad(std::istream &inputStream) override;

public:
    using iterType = decltype(numberList.begin());
    using valueType = decltype(*numberList.begin());
    using comparerType = std::function<decltype(visualValueComparer)>;
    using AlgoType = std::function<decltype(ALGORITHM(quick_sort))>;

    //The function used for running the sorting algorithm
    AlgoType algorithmFunc;

    template <typename A = std::string &, typename B = std::string &>
    AlgorithmRenderer(A name, B description, AlgoType &&algorithm)
        : OptionRenderer(), _name(std::forward<A>(name)),
            _description(std::forward<B>(description)),
            algorithmFunc(std::move(algorithm)) {
        createStarterList();
    }

    //Called once every frame
    void update(double dt) override;

    //Called to render the number list to the screen
    void render() override;

    //Gets the name of the algorithm
    const std::string &getName() const override;
};