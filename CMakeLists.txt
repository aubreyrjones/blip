#========================
#      Global Config
#========================

# this is designed to build using gcc on linux, clang on osx, and MinGW-64 on Windows.
# msvc is not currently supported, because I'm tired of its half-assed C++11 support.
# This project has not been tested on OSX, but why wouldn't it work fine? :)

cmake_minimum_required(VERSION 2.8.12)
project(blip)

# C++11 and some fairly ubiquitous vector extensions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -ffast-math -mavx -mavx2 -msse2")

#========================
#      Third Party
#========================

# disable all the tests, examples, and extraneous options

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "build glfw examples")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "build glfw docs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "build glfw tests")
add_subdirectory(glfw)

set(BUILD_PYTHON OFF CACHE BOOL "Build PyRtAudio python bindings")
set(BUILD_TESTING OFF CACHE BOOL "build rtaudio tests")
add_subdirectory(rtaudio)


#========================
#      Build Blip
#========================

set(SOURCE_FILES
        main.cpp
        glad.c)

include_directories(glm include glfw/include)

add_executable(blip ${SOURCE_FILES})

if(UNIX)
        set(SYS_LIBS dl m pthread X11 Xcursor Xi Xxf86vm GL Xrandr Xinerama)
elseif(WIN32)
        set(SYS_LIBS gdi32)
endif()

target_link_libraries(blip glfw rtaudio_static ${SYS_LIBS})