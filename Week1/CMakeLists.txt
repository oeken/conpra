cmake_minimum_required(VERSION 3.8)
project(Week1)

set(CMAKE_CXX_STANDARD 11)

add_executable(A 1A/1A.cpp)
add_executable(B 1B/1B.cpp)
add_executable(C 1C/1C.cpp)
add_executable(D 1D/1D.cpp)
add_executable(E 1E/1E.cpp)

set_target_properties(A PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../1A/)
set_target_properties(B PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../1B/)
set_target_properties(C PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../1C/)
set_target_properties(D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../1D/)
set_target_properties(E PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../1E/)