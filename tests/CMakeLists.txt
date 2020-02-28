include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/kissfft)
include_directories(${CMAKE_SOURCE_DIR}/third_party/Eigen3.3.7)
include_directories(${CMAKE_SOURCE_DIR}/src/fileio)

# test dsp
add_executable(unit_test
        # dsp
        test_fft.cpp
        test_window.cpp
        test_yin.cpp
        test_dtw.cpp
        test_features.cpp
        test_delay_line.cpp
        test_lfo.cpp

        # audio basics
        test_audio_buffer.cpp

        # fileio
        test_audio_file.cpp

        # audio effect
        test_audio_effect.cpp
        )
target_link_libraries(unit_test
        gmock_main
        libaa
        )

add_test(
        NAME unit_test
        COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_test
)