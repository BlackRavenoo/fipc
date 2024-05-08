cmake_minimum_required(VERSION 3.28)
project(cpp_project VERSION 0.1.0)

set(CLI11_ROOT $ENV{CLI11_ROOT} CACHE PATH "Path to CLI11 library")
set(NLOHMANN_JSON_ROOT $ENV{NLOHMANN_JSON_ROOT} CACHE PATH "Path to nlohmann_json library")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(SOURCE_FILES
    main.cpp
    convert.cpp
    ciede2000.cpp
    palette.cpp
    cli.cpp
    image.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dev-deps)

if(CLI11_ROOT)
    link_directories(${CLI11_ROOT}/lib)
    include_directories(${CLI11_ROOT}/include)
endif()

if(NLOHMANN_JSON_ROOT)
    link_directories(${NLOHMANN_JSON_ROOT}/lib)
    include_directories(${NLOHMANN_JSON_ROOT}/include)
endif()

find_package(CLI11 REQUIRED)
find_package(nlohmann_json REQUIRED)

add_compile_options(-pthread)
add_compile_options(-ftree-vectorize)
add_link_options(-lpthread)

add_executable(cpp_project ${SOURCE_FILES})

target_link_libraries(cpp_project PRIVATE CLI11::CLI11)
target_link_libraries(cpp_project PRIVATE nlohmann_json::nlohmann_json)