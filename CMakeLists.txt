cmake_minimum_required(VERSION 3.10)

project(wavClipper)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SndFile REQUIRED)
find_package(portaudio REQUIRED)

add_executable(clipper main.c)
target_link_libraries(clipper PRIVATE SndFile::sndfile portaudio)
install(TARGETS clipper)
