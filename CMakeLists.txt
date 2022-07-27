cmake_minimum_required(VERSION 3.19)
#set(CMAKE_VERBOSE_MAKEFILE ON)
include(CMakePrintHelpers)

project(Pentago)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(Pentago main.cpp)
add_subdirectory(src)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(Pentago sfml-window sfml-graphics sfml-audio sfml-network sfml-system)



