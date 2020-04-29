cmake_minimum_required(VERSION 3.0)
set(CMAKE_TOOLCHAIN_FILE $ENV{HOME}/arm-cross-comp-env/rpi-zero-w-toolchain.cmake)
project(SETR_TP5)

add_executable(playsound main.c wave.c)
target_link_libraries(playsound asound)

add_executable(server server.c wave.c)
target_link_libraries(server asound)

