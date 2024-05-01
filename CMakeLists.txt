cmake_minimum_required(VERSION 3.10)
project(cpp_project VERSION 1.0.0)

set(HOME_PATH "$ENV{HOME}")

set(VCPKG_ROOT "${HOME_PATH}/Git/vcpkg")

include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES main.cpp convert.cpp ciede2000.cpp palette.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


link_directories(${VCPKG_ROOT}/installed/x64-linux/include)

find_package(OpenCV REQUIRED)
find_package(CLI11 REQUIRED)
find_package(nlohmann_json REQUIRED)


add_executable(cpp_project ${SOURCE_FILES})

target_link_libraries(cpp_project PRIVATE CLI11::CLI11)
target_link_libraries(cpp_project PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(cpp_project PRIVATE ${OpenCV_LIBS})