
cmake_minimum_required(VERSION 3.17)
project(optimath C)

set(CMAKE_C_STANDARD 11)

add_executable(lab3 lab3.c)
target_link_libraries(lab3 m)