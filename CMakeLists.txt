cmake_minimum_required(VERSION 3.6)

project(AlgoVisual VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")

set(IMGUI_WITH_IMPL OFF)

add_library(StructsAndAlgorithms 
"StructsAndAlgorithms/include/binary_search_tree.h"
"StructsAndAlgorithms/include/linked_list.h" "StructsAndAlgorithms/include/common_traits.h" "StructsAndAlgorithms/src/common_traits.cpp" "StructsAndAlgorithms/include/graph.h")

target_include_directories(StructsAndAlgorithms PUBLIC "StructsAndAlgorithms/include")
set_target_properties(StructsAndAlgorithms PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} 
"Visualizer/src/main.cpp" 
"Visualizer/include/test.h"
"Visualizer/include/Graphics/Button.h" 
"Visualizer/src/Graphics/Button.cpp" 
"Visualizer/src/event.cpp" 
"Visualizer/include/Event.h"  
"Visualizer/include/render_list.h" 
"Visualizer/include/update_list.h" 
"Visualizer/include/misc.h" 
"Visualizer/src/misc.cpp"
"Visualizer/include/rlImGui.h"
"Visualizer/src/rlImGui.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC "Visualizer/include")

add_subdirectory(lib/raylib)
add_subdirectory(lib/imgui)

target_link_libraries(${PROJECT_NAME} raylib_static imgui StructsAndAlgorithms)




# TESTS
set(gtest_force_shared_crt on)
add_subdirectory(lib/googletest)

add_library(GTEST::GTEST INTERFACE IMPORTED)
target_link_libraries(GTEST::GTEST INTERFACE gtest_main)

add_executable(AlgoTests 
"Tests/src/linked_list_tests.cpp"
"tests/src/binary_search_tree_tests.cpp" "tests/src/graph_tests.cpp")

target_link_libraries(AlgoTests PRIVATE GTEST::GTEST StructsAndAlgorithms)