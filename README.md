# AlgoVisual

AlgoVisual is a C++ program that showcases various data structures and sorting algorithms. The program implements data structures such as binary search trees, linked lists, and graphs and applies sorting algorithms to them. It also includes performance metrics for quick_sort, insertion_sort, and binary_search_tree and displays each data structure graphically. The program uses GTest for unit testing each data structure and algorithm.

Use Arrow keys or WASD to move the camera around.

When you are in graph mode, you can select any node to move it around

# Building AlgoVisual

To build AlgoVisual, you need to have CMake installed. Here are the steps to build AlgoVisual:
```bash
    #1 : Clone the repository
    git clone https://github.com/nickc01/AlgoVisual.git --recursive

    #2 : Navigate to the root directory of the project
    cd AlgoVisual

    #3 : Create build directory
    mkdir build && cd build

    #4 : Run CMake
    cmake ..

    #5 : Build the project
    cmake --build .
```

The executable will be located in build/bin/ directory.
# Dependencies

AlgoVisual has the following dependencies:

- Raylib
- Imgui
- NativeFileDialog

These dependencies are included as submodules in the project and will be built automatically when building the project.
# Running AlgoVisual

Once built, you can run AlgoVisual by executing the binary located in build/bin/ directory.
# Running Tests

You can run the tests by executing the binary AlgoTests located in build/ directory.
# License

AlgoVisual is released under the MIT License. Please see the LICENSE file for details.
