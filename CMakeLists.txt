#========================
#      Global Config
#========================

# this is designed to build using gcc on linux, clang on osx, and MinGW-64 on Windows.
# msvc is not currently supported, because I'm tired of its half-assed C++11 support.
# This project has not been tested on OSX, but why wouldn't it work fine? :)

cmake_minimum_required(VERSION 2.8.12)
project(blip)

# C++11 and some fairly ubiquitous vector extensions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Os -Wall -Wno-unused-but-set-variable -Wno-multichar -ffast-math -mavx -mavx2 -msse2")

#========================
#      Third Party
#========================

# disable all the tests, examples, and extraneous options

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "build glfw examples")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "build glfw docs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "build glfw tests")
add_subdirectory(glfw)

set(DSP_FILTERS_SOURCE_FILES
        DSPFilters/shared/DSPFilters/source/Bessel.cpp
        DSPFilters/shared/DSPFilters/source/Biquad.cpp
        DSPFilters/shared/DSPFilters/source/Butterworth.cpp
        DSPFilters/shared/DSPFilters/source/Cascade.cpp
        DSPFilters/shared/DSPFilters/source/ChebyshevI.cpp
        DSPFilters/shared/DSPFilters/source/ChebyshevII.cpp
        DSPFilters/shared/DSPFilters/source/Custom.cpp
        DSPFilters/shared/DSPFilters/source/Design.cpp
        DSPFilters/shared/DSPFilters/source/Elliptic.cpp
        DSPFilters/shared/DSPFilters/source/Filter.cpp
        DSPFilters/shared/DSPFilters/source/Legendre.cpp
        DSPFilters/shared/DSPFilters/source/Param.cpp
        DSPFilters/shared/DSPFilters/source/PoleFilter.cpp
        DSPFilters/shared/DSPFilters/source/RBJ.cpp
        DSPFilters/shared/DSPFilters/source/RootFinder.cpp
        DSPFilters/shared/DSPFilters/source/State.cpp)
if(UNIX)
    add_definitions(-D__LINUX_PULSE__)
endif()
set(MAXIMILIAN_SOURCE_FILES Maximilian/maximilian.h Maximilian/maximilian.cpp)

set(RTAUDIO_SOURCE_FILES rtaudio/RtAudio.cpp rtaudio/RtAudio.h)

set(CPP_MMF_SOURCE_FILES
        cpp-mmf/memory_mapped_file.hpp
        cpp-mmf/memory_mapped_file.cpp)

set(R8BRAIN_SOURCE_FILES r8brain/r8bbase.cpp)

#========================
#      Build Blip
#========================

include_directories(glm include rtaudio glfw/include DSPFilters/shared/DSPFilters/include Maximilian cpp-mmf r8brain)

# resources

file(GLOB RESOURCE_FILES ${CMAKE_SOURCE_DIR}/assets/*)

add_custom_command(OUTPUT "assets.h" "assets.cpp"
        COMMAND "python" ARGS ${CMAKE_SOURCE_DIR}/resc "--outdir" ${CMAKE_BINARY_DIR} "assets" ${RESOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/resc ${RESOURCE_FILES}
        VERBATIM)


set(SOURCE_FILES
        src/main.cpp
        src/glad.c
        src/WavReader.cpp
        src/WavReader.h
        src/LocklessRing.h
        ${CMAKE_BINARY_DIR}/assets.h
        ${CMAKE_BINARY_DIR}/assets.cpp
        src/AudioTransport.cpp
        src/AudioTransport.h
        ${RTAUDIO_SOURCE_FILES}
        src/AudioTypes.h src/Resampler.cpp src/Resampler.h)


add_executable(blip
        ${RTAUDIO_SOURCE_FILES}
#        ${DSP_FILTERS_SOURCE_FILES}
#        ${MAXIMILIAN_SOURCE_FILES}
        ${CPP_MMF_SOURCE_FILES}
        ${R8BRAIN_SOURCE_FILES}
        ${SOURCE_FILES})

if (UNIX)
    set(SYS_LIBS dl m Xcursor Xi Xxf86vm Xrandr Xinerama X11 pulse-simple pulse pthread GL)
elseif(WIN32)
    set(SYS_LIBS gdi32)
endif ()

target_link_libraries(blip glfw ${SYS_LIBS})
