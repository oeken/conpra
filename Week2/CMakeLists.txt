cmake_minimum_required(VERSION 3.8)
project(Week2)

set(CMAKE_CXX_STANDARD 11)

add_executable(A 2A/2A.cpp)
add_executable(B 2B/2B.cpp)
add_executable(C 2C/2C.cpp)
add_executable(D 2D/2D.cpp)
add_executable(E 2E/2E.cpp)

set_target_properties(A PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../2A/)
set_target_properties(B PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../2B/)
set_target_properties(C PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../2C/)
set_target_properties(D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../2D/)
set_target_properties(E PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../2E/)