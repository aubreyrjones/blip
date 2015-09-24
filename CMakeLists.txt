cmake_minimum_required(VERSION 2.8.12)
project(blip)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -mavx -mavx2 -msse2")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "build glfw examples")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "build glfw docs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "build glfw tests")


add_subdirectory(glfw)

set(BUILD_PYTHON OFF CACHE BOOL "Build PyRtAudio python bindings")
set(BUILD_TESTING OFF CACHE BOOL "blahblahblah")

add_subdirectory(rtaudio)

set(SOURCE_FILES
        main.cpp)

include_directories(glm/)

add_executable(blip ${SOURCE_FILES})
target_link_libraries(blip glfw rtaudio_static)