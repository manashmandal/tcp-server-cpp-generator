cmake_minimum_required(VERSION 3.5)

project(TCPTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-fcoroutines)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(TCPTask main.cpp Generator.cpp Generator.h)

target_link_libraries(TCPTask ${Boost_LIBRARIES})

