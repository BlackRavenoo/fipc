cmake_minimum_required(VERSION 3.28)
project(cpp_project VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dev-deps)

add_compile_options(-pthread)
add_compile_options(-ftree-vectorize)
add_link_options(-lpthread)

add_executable(cpp_project ${SOURCE_FILES})