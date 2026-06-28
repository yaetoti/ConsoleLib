cmake_minimum_required(VERSION 3.22)
project(ConsoleLib)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp" "src/*.cu")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
