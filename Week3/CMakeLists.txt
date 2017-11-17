cmake_minimum_required(VERSION 3.8)
project(Week3)

set(CMAKE_CXX_STANDARD 11)

add_executable(A 3A/3A.cpp)
add_executable(B 3B/3B.cpp)
add_executable(C 3C/3C.cpp)
add_executable(D 3D/3D.cpp)
add_executable(E 3E/3E.cpp)

set_target_properties(A PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../3A/)
set_target_properties(B PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../3B/)
set_target_properties(C PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../3C/)
set_target_properties(D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../3D/)
set_target_properties(E PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../3E/)