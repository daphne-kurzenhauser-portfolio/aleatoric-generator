cmake_minimum_required(VERSION 3.10)

project(aleatoric)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SndFile REQUIRED)
find_package(portaudio REQUIRED)

set(ALEA_SRC 
  utils/music_utils.c
)
set(ALEA_HEADERS 
  utils/music_utils.h 
  utils/music_tbls.h
)

add_library(Alea_lib STATIC ${ALEA_HEADERS} ${ALEA_SRC})

add_executable(aleatoric main.c ${ALEA_HEADERS})
target_link_libraries(aleatoric PRIVATE Alea_lib SndFile::sndfile portaudio)
install(TARGETS aleatoric)
