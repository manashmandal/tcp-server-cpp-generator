cmake_minimum_required(VERSION 3.5)

project(TCPTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-fcoroutines)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)


add_executable(TCPTask main.cpp Generator.cpp Generator.h Database.h)

target_link_libraries(TCPTask ${Boost_LIBRARIES})
target_link_libraries(TCPTask ${Boost_LIBRARIES} $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,
        mongo::mongocxx_static,mongo::mongocxx_shared> $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)


