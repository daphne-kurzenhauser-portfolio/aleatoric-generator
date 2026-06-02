cmake_minimum_required(VERSION 3.10)

project(aleatoric)

list(APPEND CMAKE_MODULE_PATH)

find_package(SndFile REQUIRED)
find_package(portaudio REQUIRED)

set(ALEA_SRC 
  utils/music_utils.c
  utils/wav_utils.c
)
set(ALEA_HEADERS 
  utils/music_utils.h 
  utils/music_tbls.h
  utils/wav_utils.h
)

add_library(Alea_lib STATIC ${ALEA_HEADERS} ${ALEA_SRC})

add_executable(${PROJECT_NAME} main.c ${ALEA_HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE Alea_lib SndFile::sndfile portaudio)
