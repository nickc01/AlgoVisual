cmake_minimum_required(VERSION 3.6)

project(AlgoVisual VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} 
"src/main.cpp" 
"include/AlgoVisual/test.h"   
"include/AlgoVisual/DataStructures/linked_list_old.h" 
"include/AlgoVisual/DataStructures/linked_list.h")

target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(lib/raylib)

target_link_libraries(${PROJECT_NAME} raylib_static)