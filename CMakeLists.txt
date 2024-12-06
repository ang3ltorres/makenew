cmake_minimum_required(VERSION 3.31)
project(makenew LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(makenew "${CMAKE_CURRENT_SOURCE_DIR}/makenew.cpp")
target_compile_options(makenew PRIVATE -Wall -Wextra -Wpedantic -fmax-errors=1)
target_link_libraries(makenew PRIVATE stdc++exp)

