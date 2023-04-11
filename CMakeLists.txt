cmake_minimum_required(VERSION 3.6)

project(AlgoVisual VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")

set(IMGUI_WITH_IMPL OFF)


add_library(StructsAndAlgorithms 
"StructsAndAlgorithms/include/binary_search_tree.h"
"StructsAndAlgorithms/include/linked_list.h" 
"StructsAndAlgorithms/include/common.h" 
"StructsAndAlgorithms/src/common.cpp" 
"StructsAndAlgorithms/include/graph.h"
"StructsAndAlgorithms/include/struct_exception.h"
"StructsAndAlgorithms/src/struct_exception.cpp"  "StructsAndAlgorithms/include/insertion_sort.h" "StructsAndAlgorithms/include/merge_sort.h" "StructsAndAlgorithms/include/bubble_sort.h" "StructsAndAlgorithms/include/quick_sort.h" "StructsAndAlgorithms/include/heap_sort.h")

target_include_directories(StructsAndAlgorithms PUBLIC "StructsAndAlgorithms/include")
set_target_properties(StructsAndAlgorithms PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} 
"Visualizer/src/main.cpp" 
"Visualizer/include/rlImGui.h"
"Visualizer/src/rlImGui.cpp"
"Visualizer/include/OptionRenderers/OptionRenderer.h"
"Visualizer/src/OptionRenderers/OptionRenderer.cpp"
"Visualizer/include/OptionRenderers/AlgorithmRenderer.h"
"Visualizer/src/OptionRenderers/AlgorithmRenderer.cpp"
"Visualizer/include/OptionRenderers/BinarySearchTreeRenderer.h"
"Visualizer/src/OptionRenderers/BinarySearchTreeRenderer.cpp"
"Visualizer/include/OptionRenderers/GraphRenderer.h"
"Visualizer/src/OptionRenderers/GraphRenderer.cpp"
"Visualizer/src/global.cpp"
"Visualizer/include/global.h"
"Visualizer/include/visual_container.h")


target_include_directories(${PROJECT_NAME} PUBLIC "Visualizer/include")

add_subdirectory(lib/raylib)
add_subdirectory(lib/imgui)
add_subdirectory(lib/nativefiledialog)

target_link_libraries(${PROJECT_NAME} raylib_static imgui StructsAndAlgorithms nfd)

file(GLOB_RECURSE RESOURCE_FILES
     "resources/*.*"
)

foreach(file ${RESOURCE_FILES})
	file(RELATIVE_PATH CURRENT_RESOURCE_PATH ${CMAKE_SOURCE_DIR} ${file})

	get_filename_component(CURRENT_RESOURCE_NAME ${file} NAME_WE)
	string(TOUPPER ${CURRENT_RESOURCE_NAME} CURRENT_RESOURCE_NAME)
	target_compile_definitions(${PROJECT_NAME} PRIVATE RES_${CURRENT_RESOURCE_NAME}="${CURRENT_RESOURCE_PATH}")
endforeach()


# TESTS
set(gtest_force_shared_crt on)
add_subdirectory(lib/googletest)

add_library(GTEST::GTEST INTERFACE IMPORTED)
target_link_libraries(GTEST::GTEST INTERFACE gtest_main)

add_executable(AlgoTests 
"test/src/linked_list_tests.cpp"
"test/src/binary_search_tree_tests.cpp" 
"test/src/graph_tests.cpp"
"test/src/algorithm_tests.cpp")

target_link_libraries(AlgoTests PRIVATE GTEST::GTEST StructsAndAlgorithms)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources/ ${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}/resources)