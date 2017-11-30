cmake_minimum_required(VERSION 3.8)
project(Week5)

set(CMAKE_CXX_STANDARD 11)

add_executable(A 5A/5A.cpp)
add_executable(A_edmondskarp 5A/5A_edmondskarp.cpp)
add_executable(A_pr 5A/5A_pr.cpp)

add_executable(B 5B/5B.cpp)
add_executable(B_pr 5B/5B_pr.cpp)

add_executable(C 5C/5C.cpp)
add_executable(C_pr 5C/5C_pr.cpp)

add_executable(D 5D/5D.cpp)

add_executable(E 5E/5E.cpp)
add_executable(E_pr 5E/5E_pr.cpp)
add_executable(E_pr2 5E/5E_pr2.cpp)

set_target_properties(A PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5A/)
set_target_properties(A_edmondskarp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5A/)
set_target_properties(A_pr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5A/)

set_target_properties(B PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5B/)
set_target_properties(B_pr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5B/)

set_target_properties(C PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5C/)
set_target_properties(C_pr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5C/)

set_target_properties(D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5D/)

set_target_properties(E PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5E/)
set_target_properties(E_pr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5E/)
set_target_properties(E_pr2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../5E/)