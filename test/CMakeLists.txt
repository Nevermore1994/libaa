include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/kissfft)
include_directories(${CMAKE_SOURCE_DIR}/third_party/Eigen3.3.7)

message(STATUS ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_INSTALL_BINDIR:${CMAKE_INSTALL_BINDIR}")


add_executable(test_dsp_main
        test_dsp_main.cpp
        test_fft.cpp
        test_stft.cpp
        test_window.cpp
        test_yin.cpp
        )
target_link_libraries(test_dsp_main
        gmock_main
        libaa
        )

add_test(
        NAME test_dsp_main
        COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test_dsp_main
)