cmake_minimum_required(VERSION 3.8)
project(Week4)

set(CMAKE_CXX_STANDARD 11)

add_executable(A 4A/4A.cpp)
add_executable(B 4B/4B.cpp)
add_executable(C 4C/4C.cpp)
add_executable(D 4D/4D.cpp)
add_executable(E 4E/4E.cpp)

set_target_properties(A PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../4A/)
set_target_properties(B PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../4B/)
set_target_properties(C PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../4C/)
set_target_properties(D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../4D/)
set_target_properties(E PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../4E/)