include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/kissfft)

message(STATUS ${CMAKE_SOURCE_DIR})

add_executable(test_dsp_main
        test_dsp_main.cpp
        test_fft.cpp
        )
target_link_libraries(test_dsp_main
        gmock_main
        libaa
        )

