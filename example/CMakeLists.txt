
add_executable(load_file load_file.cpp)
target_link_libraries(load_file libaa)

add_executable(stft stft.cpp)
target_link_libraries(stft libaa)