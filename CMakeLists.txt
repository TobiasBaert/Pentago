cmake_minimum_required(VERSION 3.19)
#set(CMAKE_VERBOSE_MAKEFILE ON)
include(CMakePrintHelpers)

project(Pentago)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(Pentago main.cpp)
add_subdirectory(src)

# for lakshayg / compile_time library
include_directories(compile_time/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(Pentago sfml-window sfml-graphics sfml-audio sfml-network sfml-system)



