cmake_minimum_required(VERSION 3.10)

project(libaa)

set(CMAKE_CXX_STANDARD 14)

# must return type
add_compile_options(-Werror=return-type)

add_subdirectory(third_party)

include_directories(include)
include_directories(third_party/dr_libs)
include_directories(third_party/kissfft)
include_directories(third_party/Eigen3.3.7)

set(FILEIO_SRC
        src/fileio/aa_wav_decoder.cpp
        src/fileio/aa_audio_file.cpp)

set(DSP_SR
        ${CMAKE_SOURCE_DIR}/src/dsp/aa_fft.cpp
        ${CMAKE_SOURCE_DIR}/third_party/kissfft/kiss_fft.c
        ${CMAKE_SOURCE_DIR}/third_party/kissfft/kiss_fftr.c
        ${CMAKE_SOURCE_DIR}/src/dsp/aa_stft.cpp
        ${CMAKE_SOURCE_DIR}/src/dsp/aa_window.cpp
        ${CMAKE_SOURCE_DIR}/src/dsp/aa_mfcc.cpp
        )

add_library(libaa ${FILEIO_SRC} ${DSP_SR})

add_subdirectory(example)
add_subdirectory(test)