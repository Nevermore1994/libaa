cmake_minimum_required(VERSION 3.10)

project(libaa)

set(CMAKE_CXX_STANDARD 14)

# must return type
add_compile_options(-Werror=return-type)

add_subdirectory(third_party)

include_directories(include)
include_directories(third_party/dr_libs)
include_directories(third_party/kissfft)

set(FILEIO_SRC
        src/fileio/aa_wav_decoder.cpp
        src/fileio/aa_audio_file.cpp)

set(DSP_SR
        src/dsp/aa_fft.cpp
        third_party/kissfft/kiss_fft.c
        third_party/kissfft/kiss_fftr.c)

add_library(libaa ${FILEIO_SRC} ${DSP_SR})

add_subdirectory(example)
add_subdirectory(test)