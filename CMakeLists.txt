cmake_minimum_required(VERSION 3.6)

project(AlgoVisual VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")

set(IMGUI_WITH_IMPL OFF)

add_executable(${PROJECT_NAME} 
"src/main.cpp" 
"include/AlgoVisual/test.h"   
 
"include/AlgoVisual/DataStructures/linked_list.h" 
"include/AlgoVisual/Graphics/Button.h" 
"src/Graphics/Button.cpp" 
"src/event.cpp" 
"include/AlgoVisual/Event.h"  
"include/AlgoVisual/render_list.h" 
"include/AlgoVisual/update_list.h" 
"include/AlgoVisual/misc.h" 
"src/misc.cpp"
"include/AlgoVisual/rlImGui.h"
"src/rlImGui.cpp" "include/AlgoVisual/DataStructures/binary_search_tree.h")

target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(lib/raylib)
add_subdirectory(lib/imgui)

target_link_libraries(${PROJECT_NAME} raylib_static imgui)




# TESTS
# include(CTest)
#add_subdirectory(lib/googletest)
#add_executable(AlgoTests "tests/src/linked_list_tests.cpp")

#target_link_libraries(AlgoTests PRIVATE GTest::GTest AlgoVisual)