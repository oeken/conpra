cmake_minimum_required(VERSION 3.8)
project(Week7)

set(CMAKE_CXX_STANDARD 11)

add_executable(A 7A/7A.cpp)
add_executable(B 7B/7B.cpp)
add_executable(C 7C/7C.cpp)
add_executable(D 7D/7D.cpp)
add_executable(E 7E/7E.cpp)

set_target_properties(A PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../7A/)
set_target_properties(B PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../7B/)
set_target_properties(C PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../7C/)
set_target_properties(D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../7D/)
set_target_properties(E PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../7E/)