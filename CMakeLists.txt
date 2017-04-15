cmake_minimum_required(VERSION 3.0)

project(Sudokode)

set(SOURCE src/sudokode.cpp)

set (CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} ${SOURCE})