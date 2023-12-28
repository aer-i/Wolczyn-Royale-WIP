cmake_minimum_required(VERSION 3.6)
project(LightFrame)

set(CMAKE_CXX_STANDARD 20)

add_executable(LightFrame src/main.cpp)

add_subdirectory(vendor/ARLN)
target_link_libraries(LightFrame PUBLIC ARLN)