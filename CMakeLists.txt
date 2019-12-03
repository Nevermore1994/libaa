cmake_minimum_required(VERSION 3.10)

project(libaa)

set(CMAKE_CXX_STANDARD 14)

# must return type
add_compile_options(-Werror=return-type)

add_subdirectory(third_party)

include_directories(include)
include_directories(third_party/dr_libs)

set(FILEIO_SRC
        src/aa_wav_decoder.cpp
        src/aa_audio_file.cpp)

add_library(libaa ${FILEIO_SRC})

add_subdirectory(example)